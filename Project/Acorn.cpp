/**
 *	@file MobMoleMadness\Acorn.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright � 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Acorn.h"
#include "Circle.h"
#include "Box2D.h"
#include "Mole.h"

#include "MMMContext.h"

#include "GDE/Core/Rect.h"

#include "GDE/Renderer/Sprite.h"
#include "GDE/Renderer/SpriteManager.h"
#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"


#include <cmath>


CAcorn::CAcorn()
: m_pSprite(0)
, m_bHighlighted(0)
, m_eState(ES_Available)
, m_bBeenFound(false)
, m_pOwnerMole(0)
{
};


CAcorn::~CAcorn( void )
{
	if ( m_pSprite )
	{
		m_pSprite->Release();
	}
}


/**
 *	Initialise the acorn at a given position and texture name, returning succes.
 */
bool CAcorn::Init( const CFVec2& vPosition, const char *strTexture )
{
	m_eState = ES_Available;

	m_bHighlighted = false;
	m_vPosition = vPosition;
	m_pSprite = CMMMContext::GetInstance().GetSpriteM()->FindResouceById( GDE::CResourceId( strTexture ) );
	if ( NULL == m_pSprite )
	{
		GDE::CTexture* pTexture = CMMMContext::GetInstance().GetRenderer()->GetTextureManager().LoadResource( strTexture );
		if ( NULL == pTexture )
		{
			assert_now( "Unable to find Image \"%s\" for acorn", strTexture );
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

void CAcorn::Draw( void )
{
	if ( m_eState != ES_Available )
	{
		return;
	}
	m_pSprite->Render( m_vPosition.X(), m_vPosition.Y() );
	if ( m_bHighlighted )
	{
		CCircle Bounds;
		Bounds.Initialise( m_vPosition, fACORN_RADIUS );
		CMMMContext::GetInstance().DrawCircle( Bounds, 0xFFFF0000 );
	}
}

bool CAcorn::Intersects( const CCircle& Circle ) const
{
	CCircle Bounds;
	Bounds.Initialise( m_vPosition, fACORN_RADIUS );
	return Circle.Overlaps( Bounds );
}

bool CAcorn::Intersects( const CBox2D& Box ) const
{
	CCircle Bounds;
	Bounds.Initialise( m_vPosition, fACORN_RADIUS );
	return Box.Intersects( Bounds );
}

void CAcorn::SetState(EState eState)
{
#ifndef _EDITOR
	CMole* pMoles = 0;
	int iNumMoles = 0;


	CMMMContext::GetInstance().GetMoles(pMoles, iNumMoles);


	if (m_eState == ES_Carried &&
		eState == ES_Available)
	{
		CMole::NotifyAcornDropped(this);
	}
	else
	{
		// Do nothing
	}
#endif


	m_eState = eState;
}


void
CAcorn::Update(CMole* _pMoles, int _iNumMoles)
{
	_pMoles;
	_iNumMoles;
#ifndef _EDITOR
	if (!m_bBeenFound)
	{
		for (int i = 0; i < _iNumMoles; ++ i)
		{
			if (sqrt(_pMoles[i].GetPosition().SquareDistance(m_vPosition)) - fMOLE_RADIUS - 13.0f <= 0.0f)
			{
				_pMoles[i].NotifyAcorn(this);
				m_bBeenFound = true;
				break;
			}
		}
	}
#endif
}