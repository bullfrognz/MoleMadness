/**
 *	@file MobMoleMadness\Squirrel.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Squirrel.h"
#include "Circle.h"
#include "MMMContext.h"
#include "Tunnel.h"
#include "Acorn.h"

#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"

#include "GDE/DataParser/DataParser.h"
#include "GDE/DataParser/DataParserManager.h"


#include "GDE/Core/Rect.h"

#include "GDE/Renderer/Sprite.h"
#include "GDE/Renderer/SpriteManager.h"
#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"

#include <stdlib.h>

CSquirrel::~CSquirrel( void )
{
	if ( m_pSprite )
	{
		m_pSprite->Release();
		m_pSprite = NULL;
	}
	if ( m_ppAcorns )
	{
		delete[] m_ppAcorns;
		m_ppAcorns = NULL;
	}
}


/**
 *	Initialise the squirrel from the provided parser.
 */
bool CSquirrel::Init( GDE::CDataParser* pParser )
{
	m_fMinDistSep = 0.2f;
	m_fMaxDistSep = 1.0f;
	m_fTimeToDisturbance = 0.0f;
	m_fLastDistDelay = 0.0f;
	m_fDistTimeMoving = 0.0f;
	m_fMinDistRad = 0.0f;
	m_fMaxDistRad = 200.0f;

	m_uScore = 0;
	m_ppAcorns = NULL;
	m_uNumAcorns = 0;
	m_vPosition = SFVec2( 100.0f, 100.f );
	m_pSprite = NULL;
	/**
	 *	Tokens to look up in the parser for each movement.
	 */
	const static char* s_strMoveTokens[] = 
	{
		"Left",
		"Right",
		"Up",
		"Down"
	};
	_COMPILE_ASSERT( _ARRAY_SIZE( s_strMoveTokens ) == EMove_COUNT );

	if ( false == pParser->GetValue( m_fSpeed, "Squirrel", "Speed" ) )
	{
		assert_now( "Unable to parse squirrel speed" );
		return false;
	}

	if ( false == pParser->GetValue( m_fRadius, "Squirrel", "Radius" ) )
	{
		assert_now( "Unable to parse squirrel radius" );
		return false;
	}

	if ( false == pParser->GetValue( m_uMaxLives, "Squirrel", "MaxLives" ) )
	{
		assert_now( "Unable to parse maximum squirrel lives" );
		return false;
	}
	m_uNumLives = m_uMaxLives;

	if ( false == pParser->GetValue( m_uMaxAcorns, "Squirrel", "MaxAcorns" ) )
	{
		assert_now( "Unable to parse maximum squirrel acorns" );
		return false;
	}
	m_ppAcorns = new CAcorn*[m_uMaxAcorns];

	if ( false == pParser->GetValue( m_fMinDistSep, "Squirrel", "MinDisturbanceSep" ) )
	{
		assert_now( "Unable to parse squirrel MinDisturbanceSep" );
		return false;
	}
	if ( false == pParser->GetValue( m_fMaxDistSep, "Squirrel", "MaxDisturbanceSep" ) )
	{
		assert_now( "Unable to parse squirrel MaxDisturbanceSep" );
		return false;
	}
	if ( false == pParser->GetValue( m_fMinDistRad, "Squirrel", "MinDisturbanceRad" ) )
	{
		assert_now( "Unable to parse squirrel MinDisturbanceRad" );
		return false;
	}
	if ( false == pParser->GetValue( m_fMaxDistRad, "Squirrel", "MaxDisturbanceRad" ) )
	{
		assert_now( "Unable to parse squirrel MaxDisturbanceRad" );
		return false;
	}

	// parse and get input bindings:
	const static UINT32 uMAX_BINDING_LEN = 32;
	char strInputBinding[uMAX_BINDING_LEN+1];
	GDE::CInputDevice* pDevice = CMMMContext::GetInstance().GetInput();
	for ( UINT32 i=0; i<EMove_COUNT; i++ )
	{
		m_Movements[i].m_bValue = false;
		if ( false == pParser->GetString( strInputBinding, uMAX_BINDING_LEN, "Squirrel", s_strMoveTokens[i] ) )
		{
			assert_now( "Unable to find squirrel movement binding for %s", s_strMoveTokens[i] );
			return false;
		}
		// now get code
		if ( false == pDevice->GetInputCode( strInputBinding, m_Movements[i].m_uInputCode ) )
		{
			assert_now( "Unable to bind squirrel input \"%s\" for movement %s", strInputBinding, s_strMoveTokens[i] );
			return false;
		}
	}

	const static UINT32 uMAX_SPRITENAME_LEN = 128;
	char strTexture[uMAX_SPRITENAME_LEN];
	if ( false == pParser->GetString( strTexture, uMAX_SPRITENAME_LEN, "Squirrel", "Sprite" ) )
	{
		assert_now( "Unable to find squirrel sprite name" );
		return false;
	}

	m_pSprite = CMMMContext::GetInstance().GetSpriteM()->FindResouceById( GDE::CResourceId( strTexture ) );
	if ( NULL == m_pSprite )
	{
		GDE::CTexture* pTexture = CMMMContext::GetInstance().GetRenderer()->GetTextureManager().LoadResource( strTexture );
		if ( NULL == pTexture )
		{
			assert_now( "Unable to find Image \"%s\" for Squirrel", strTexture );
			return false;
		}

		GDE::SFRect TexRect;
		TexRect.m_fX = 0.0f;
		TexRect.m_fY = 0.0f;
		TexRect.m_fWidth = (float)pTexture->GetWidth();
		TexRect.m_fHeight = (float)pTexture->GetHeight();
		m_pSprite = CMMMContext::GetInstance().GetSpriteM()->CreateSprite( GDE::CResourceId( strTexture ), pTexture, TexRect );
		m_pSprite->SetHotSpot( TexRect.m_fWidth*0.5f, TexRect.m_fHeight*0.5f );
		pTexture->Release();	// now held inside the sprite.
	}

	return true;
}

bool CSquirrel::RegisterInput( void )
{
	GDE::CInputDevice* pDevice = CMMMContext::GetInstance().GetInput();
	for ( UINT32 i=0; i<EMove_COUNT; i++ )
	{
		if ( false == pDevice->GetChangeSubject().AddObserver( this, m_Movements[i].m_uInputCode ) )
		{
			assert_now( "Error adding observer" );
			return false;
		}
	}
	return true;
}

void CSquirrel::ReleaseInput( void )
{
	GDE::CInputDevice* pDevice = CMMMContext::GetInstance().GetInput();
	for ( UINT32 i=0; i<EMove_COUNT; i++ )
	{
		if ( false == pDevice->GetChangeSubject().RemoveObserver( this, m_Movements[i].m_uInputCode ) )
		{
			//assert_now( "Error removing observer" );
		}
	}
}

void	CSquirrel::NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
{
	bool bDown = false;
	GDE::CInputDevice& Device = reinterpret_cast<GDE::IInputChangeSubject*>(pSubject)->GetDevice();
	if ( Device.GetState( uMessage ).m_bButtonDown )
	{
		bDown = true;
	}
	for ( UINT32 i=0; i< EMove_COUNT; i++ )
	{
		if ( uMessage == m_Movements[i].m_uInputCode )
		{
			m_Movements[i].m_bValue = bDown;
			return;
		}
	}
	assert_now( "Unable to find input mapping!" );
}

void	CSquirrel::Spawn( void )
{
	CTunnel*	pTunnels;
	UINT32		uNumTunnels;
	CMMMContext::GetInstance().GetTunnels( pTunnels, uNumTunnels );
	// get a random tunnel:
	UINT32 uTunnelNum = rand()%uNumTunnels;
	m_vPosition = pTunnels[uTunnelNum].GetPosition();
	m_fDistTimeMoving = 0.0f;

	StopMoving();
}

void	CSquirrel::Update( float fTimeDelta )
{
	if ( 0 == m_uNumLives )
	{
		return;
	}

	const static CFVec2 s_vMoveOffsets[] = 
	{
		SFVec2( -1.0f,  0.0f ), //"Left",
		SFVec2(  1.0f,  0.0f ), //"Right",
		SFVec2(  0.0f, -1.0f ), //"Up",
		SFVec2(  0.0f,  1.0f ), //"Down"
	};

	_COMPILE_ASSERT( _ARRAY_SIZE( s_vMoveOffsets ) == EMove_COUNT );

	CFVec2 vMove = SFVec2( 0.0f, 0.0f );
	for ( UINT32 i=0; i< EMove_COUNT; i++ )
	{
		if ( m_Movements[i].m_bValue )
		{
			vMove += s_vMoveOffsets[i];
		}
	}

	// great, now we have the movement direction.
	if ( vMove.SquareMagnitude() != 0.0f )
	{
		m_fDistTimeMoving += fTimeDelta;
		vMove.Normalise();
		float fStep = 1.0f;
		CFVec2 vTestedMove = vMove * fTimeDelta * m_fSpeed * fStep;
		// now check the target position -  is it embedded in any walls?
		CCircle TargetBounds;
		while ( fStep > 0.0f )
		{
			TargetBounds.Initialise( m_vPosition + vTestedMove, m_fRadius );
			if ( false == CMMMContext::GetInstance().CircleIntersectsGeometry( TargetBounds ) )
			{
				break;	// found a valid, allowable movement.
			}
			fStep -= 0.2f;
			vTestedMove = vMove * fTimeDelta * m_fSpeed * fStep;
		}
		// now update to the new position
		m_vPosition += vTestedMove;

		// finally what happens at the new position
		// is an acorn collected?
		if ( m_uNumAcorns != m_uMaxAcorns )
		{
			CAcorn* pLevelAcorns;
			UINT32 uNumLevelAcorns;
			CMMMContext::GetInstance().GetAcorns( pLevelAcorns, uNumLevelAcorns );
			for ( UINT32 i=0; i<uNumLevelAcorns; i++ )
			{
				if ( pLevelAcorns[i].GetState() == CAcorn::ES_Available )
				{
					if ( pLevelAcorns[i].Intersects( TargetBounds ) )
					{
						pLevelAcorns[i].SetState( CAcorn::ES_Carried);
						m_ppAcorns[m_uNumAcorns++] = pLevelAcorns+i;
						if ( m_uNumAcorns == m_uMaxAcorns )
						{
							break;	// cannont collect any more!
						}
					}
				}
			}
		}
		// is a tunnel reached?
		if ( m_uNumAcorns > 0 )
		{
			CTunnel*	pTunnels;
			UINT32		uNumTunnels;
			CMMMContext::GetInstance().GetTunnels( pTunnels, uNumTunnels );
			for ( UINT32 i=0; i<uNumTunnels; i++ )
			{
				if ( pTunnels[i].Intersects( TargetBounds ) )
				{
					// reached the tunnel.
					for ( UINT32 i=0; i<m_uNumAcorns; i++ )
					{
						m_ppAcorns[i]->SetState( CAcorn::ES_Collected );
						m_uScore++;
					}
					m_uNumAcorns = 0;
				}
			}
		}
	}

	m_fTimeToDisturbance -= fTimeDelta;
	if ( m_fTimeToDisturbance <= 0.0f )
	{
		// schedule the next disturbance.
		m_fTimeToDisturbance = float(rand())/float(RAND_MAX);
		m_fTimeToDisturbance *= m_fMaxDistSep-m_fMinDistSep;
		m_fTimeToDisturbance += m_fMinDistSep;

		// create this disturbance:
		float fRad = m_fLastDistDelay=0.0f?0.0f:m_fDistTimeMoving/m_fLastDistDelay;
//		DbgPrint( "Creating Disturbance strength %0.2f, next delay %0.2f\n", fRad,m_fTimeToDisturbance );
		fRad *= m_fMaxDistRad-m_fMinDistRad;
		fRad += m_fMinDistRad;

		if ( fRad >= 0.0f )
		{
			CCircle Dist;
			Dist.Initialise( m_vPosition, fRad );
			CMMMContext::GetInstance().CreateDisturbance( Dist );
		}
		m_fDistTimeMoving = 0.0f;
		m_fLastDistDelay = m_fTimeToDisturbance;
	}



}


void		CSquirrel::Draw( void )
{
	if ( m_pSprite )
	{
		m_pSprite->Render( m_vPosition.X(), m_vPosition.Y() );
	}
}

void		CSquirrel::LoseLife( void )
{
	if ( m_uNumLives )
	{
		// do not lose a life if over a tunnel:
		CTunnel*	pTunnels;
		UINT32		uNumTunnels;
		CMMMContext::GetInstance().GetTunnels( pTunnels, uNumTunnels );

		CCircle Bounds;
		Bounds.Initialise( m_vPosition, m_fRadius );
		for ( UINT32 i=0; i<uNumTunnels; i++ )
		{
			if ( pTunnels[i].Intersects( Bounds ) )
			{
				return; // immune, over a tunnel!
			}
		}


		for ( UINT32 i=0; i<m_uNumAcorns; i++ )
		{
			m_ppAcorns[i]->SetState( CAcorn::ES_Available );
		}
		m_uNumAcorns = 0;
		m_uNumLives--;
		if ( m_uNumLives )
		{
			Spawn();
		} else
		{
			CMMMContext::GetInstance().NotifySquirrelDeath();
		}
	}
}


void		CSquirrel::ResetLives( void )
{
	m_uNumLives = m_uMaxLives;	
	m_uScore = 0;
	m_uNumAcorns = 0;
}

void 
CSquirrel::StopMoving()
{
	for (int i = 0; i < EMove_COUNT; ++ i)
	{
		m_Movements[i].m_bValue = false;
	}
}

bool CSquirrel::Intersects( const CCircle& Circle ) const
{
	CCircle Bounds;
	Bounds.Initialise( m_vPosition, m_fRadius );
	return Circle.Overlaps( Bounds );
}
