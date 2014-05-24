/**
 *	@file MobMoleMadness\MMMContext.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#include "Common.h"
#include "MMMContext.h"
#include "FVector2.h"
#include "Circle.h"
#include "Mouse.h"
#include "Map.h"
#include "Wall.h"
#include "Acorn.h"
#include "Mole.h"
#include "Tunnel.h"
#include "Box2D.h"
#ifndef _EDITOR
#include "Squirrel.h"
#include "Path.h"
#endif

#include "GDE/Core/GameTimer.h"
#include "GDE/Renderer/Renderer.h"
#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Renderer/FontManager.h"
#include "GDE/Renderer/Font.h"
#include "GDE/Renderer/Vertex.h"
#include "GDE/Renderer/SpriteManager.h"
#include "GDE/Core/Logger.h"
#include "GDE/DataParser/DataParserManager.h"
#include "GDE/DataParser/DataParser.h"

// for untextured:
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"

#include <stdio.h> // for debug string info printing...
#include <math.h> // for sin/cos

/**
 *	Convenience template to delete things, allowing early returns without leaking memory.
 */
template<typename T> class TScopeDeleter
{
public:
	TScopeDeleter<T>( T* pVal )
		: m_pVal( pVal )
	{	// empty
	}
	~TScopeDeleter<T>( void )
	{
		if ( m_pVal )
		{
			delete[] m_pVal;
		}
	}
	void Release( void )
	{
		m_pVal = NULL;
	}
protected:
	T* m_pVal;
};

#ifndef _EDITOR
class CGameOverListener : public GDE::IObserver
{
	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		UNUSED_VAR( pSubject );
		UNUSED_VAR( uMessage );
		CMMMContext::GetInstance().RestartGame();
	}
};
static CGameOverListener gs_GameOverListener;

class CChangeSpeedOfTimeObserver : public GDE::IObserver
{
public:
	CChangeSpeedOfTimeObserver( bool bSpeedUp )
		: m_bSpeedUp( bSpeedUp )
	{	// empty
	}
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		UNUSED_VAR( pSubject );
		UNUSED_VAR( uMessage );
		if ( m_bSpeedUp )
		{
			float fSpeedOfTime = CMMMContext::GetInstance().GetSpeedOfTime();
			fSpeedOfTime = (fSpeedOfTime<=9.95f)?fSpeedOfTime+0.05f:fSpeedOfTime;
			CMMMContext::GetInstance().SetSpeedOfTime( fSpeedOfTime );
		} else
		{
			float fSpeedOfTime = CMMMContext::GetInstance().GetSpeedOfTime();
			fSpeedOfTime = (fSpeedOfTime>0.05f)?fSpeedOfTime-0.05f:fSpeedOfTime;
			CMMMContext::GetInstance().SetSpeedOfTime( fSpeedOfTime );
		}
	}
protected:
	CChangeSpeedOfTimeObserver& operator=(const CChangeSpeedOfTimeObserver& Other);	// non implemented assignment, fixes a warning.
	
	bool			m_bSpeedUp;
};

static CChangeSpeedOfTimeObserver gs_SpeedUpTimeObsvr( true );
static CChangeSpeedOfTimeObserver gs_SlowDownTimeObsvr( false );

class CPauseGameListener : public GDE::IObserver
{
	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		UNUSED_VAR( pSubject );
		UNUSED_VAR( uMessage );
		CMMMContext::GetInstance().SetPaused( false==CMMMContext::GetInstance().IsPaused() );
	}
};
static CPauseGameListener gs_PauseGameListener;

class CDebugAddDisturbanceListener : public GDE::IObserver
{
public:
	CDebugAddDisturbanceListener( void )
		: m_fAge( 0.0f )
		, m_bMouseDown( false )
	{
	}
	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		GDE::CInputDevice& Device = reinterpret_cast<GDE::IInputChangeSubject*>(pSubject)->GetDevice();
		if ( Device.GetState( uMessage ).m_bButtonDown )
		{
			m_bMouseDown = true;
			m_fAge = 0.0f;
		} else
		{
			if ( m_bMouseDown )
			{
				m_bMouseDown = false;
				CCircle Dist;	
				Dist.Initialise( CMMMContext::GetInstance().GetMouse()->GetPosition(), m_fAge*100.0f );
				CMMMContext::GetInstance().CreateDisturbance( Dist );
			}
		}
	}
	void UpdateAndRender( float fTimeDelta )
	{
		if ( m_bMouseDown )
		{
			m_fAge += fTimeDelta;
			CCircle Dist;	
			Dist.Initialise( CMMMContext::GetInstance().GetMouse()->GetPosition(), m_fAge*100.0f );
			CMMMContext::GetInstance().DrawCircle( Dist, 0xFF00FF00 );
		}
	}
	float m_fAge;
	bool	m_bMouseDown;
};
static CDebugAddDisturbanceListener	gs_DebugAddDisturbanceListener;

struct SDisturbanceLog
{
	CCircle	m_Circle;
	float	m_fAge;
};
#else
class CEditorChangeToolListener : public GDE::IObserver
{
	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		UNUSED_VAR( pSubject );
		UNUSED_VAR( uMessage );
		CMMMContext::GetInstance().SetEditTool( IEditorTool::GetTool( ETool_SelectTool ) );
	}
};
static CEditorChangeToolListener gs_EditorChangeToolListener;
#endif




















/**
 *	Singleton get instance.
 */
CMMMContext& CMMMContext::GetInstance( void )
{
	static CMMMContext Context;
	return Context;
}

/**
 *	Construct an empty context, ready to be initialised.
 *	Note: this is a singleton, so this is protected.
 */
CMMMContext::CMMMContext( void )
: m_pRenderer( NULL )
, m_pInputMgr( NULL )
, m_pParserManager( NULL )
, m_pSpriteManager( NULL )
, m_pFrameTimer( NULL )
, m_pGameTimer( NULL )
, m_pMouse( NULL )
, m_pMap( NULL )
, m_pWalls( NULL )
, m_uNumWalls( 0 )
, m_pAcorns( NULL )
, m_uNumAcorns( 0 )
, m_pMoles( NULL )
, m_uNumMoles( 0 )
, m_pTunnels( NULL )
, m_uNumTunnels( 0 )

#ifdef _EDITOR
, m_pEditorTool( NULL )
#else
, m_pSquirrel( NULL )
, m_fSpeedOfTime( 1.0f )
, m_iCurrentDifficulty(0)
, m_bFirstRun(true)
#endif
{	// initialisation, empty.
}

/**
 *	Initialise the context and create all resources it requires.
 *
 *	@return true on success.
 */
bool CMMMContext::Initialise( 
	GDE::CRenderer& Renderer, 
	GDE::CInputDevice& InputMgr,
	GDE::CFontManager& FontManager,
	GDE::CDataParserManager& DataParserMan,
	GDE::CSpriteManager& SpriteManager )
{
	//m_bWindowed = false;
	//m_uDispWidth = 1024;
	//m_uDispHeight = 768;
	//m_uDispWidth = 1280;
	//m_uDispHeight = 1024;
	m_strWindowTitle = "MDS GD230 - AI MobMoleMadness";

	if ( false == Renderer.SetContext( *this ) )
	{
		assert_now( "error creating context" );
		return false;
	}

	m_pRenderer = &Renderer;
	m_pInputMgr = &InputMgr;
	m_pParserManager = &DataParserMan;
	m_pSpriteManager = &SpriteManager;

	m_pFrameTimer = new GDE::CClock();
	m_pGameTimer = new GDE::CClock();

	m_pMouse = new CMouse;
	CreateAndRegisterKey( "MouseXY", m_pInputMgr->GetChangeSubject(), *m_pMouse );








#ifndef _EDITOR

	CreateAndRegisterKey( "KeyB", m_pInputMgr->GetDownSubject(), m_AddLive );
	CreateAndRegisterKey( "KeyN", m_pInputMgr->GetDownSubject(), m_SkipLevel );
	CreateAndRegisterKey( "KeyM", m_pInputMgr->GetDownSubject(), m_SleepMoles );

	

	m_pDisturbanceLog = NULL;
#endif


	m_pFont = FontManager.LoadResource( GDE::CResourceId( "CourierNew.fnt" ) );
	if ( NULL == m_pFont )
	{
		return false;
	}
	m_pFont->SetColour( 0xFF000000 );

#ifndef _EDITOR
	// now load in the ini file:
	GDE::CDataParser* pParser = m_pParserManager->LoadResource( GDE::CResourceId( "MobMoleMadness.ini" ) );
	if ( NULL == pParser )
	{
		assert_now( "Unable to load/parse ini file" );
		return false;
	}

	float fMaxFrameRate;
	if( pParser->GetValue( fMaxFrameRate, "Main", "MaxFrameRate" ) )
	{
		m_pFrameTimer->SetMinFrameInterval( 1.0f/fMaxFrameRate );
	}

	if ( false == pParser->GetValue( m_uNumLevels, "Main", "NumLevels" ) )
	{
		assert_now( "Unable to get NumLevels from Main when parsing" );
		pParser->Release();
		return false;
	}

	if ( false == pParser->GetValue( m_fPlayerBaseDelay, "Main", "BasePlayerDelay" ) )
	{
		assert_now( "Unable to get BasePlayerDelay from Main when parsing" );
		pParser->Release();
		return false;
	}

	if ( false == pParser->GetValue( m_fDisturbanceLogMaxAge, "Main", "DisturbanceLogAge" ) )
	{
		assert_now( "Unable to get disturbance log age" );
		m_fDisturbanceLogMaxAge = 0.2f;
	}

	if ( false == pParser->GetValue( m_uDisturbanceLogSize, "Main", "DisturbanceLogSize" ) )
	{
		assert_now( "Unable to get disturbance log size" );
		m_uDisturbanceLogSize = 0;
	} else
	{
		m_pDisturbanceLog = new SDisturbanceLog[m_uDisturbanceLogSize];
		for ( UINT32 i=0; i<m_uDisturbanceLogSize; i++ )
		{
			m_pDisturbanceLog[i].m_fAge = m_fDisturbanceLogMaxAge;
		}
	}

	m_pSquirrel = new CSquirrel();
	if ( false == m_pSquirrel->Init( pParser ) )
	{
		assert_now( "Error creating squirrel" );
		pParser->Release();
	}
	m_pSquirrel->ResetLives();
	m_pSquirrel->RegisterInput();
	pParser->Release();

	if ( false == LoadLevel( 0 ) )
	{
		assert_now( "Unable to load level 0" );
		pParser->Release();
		return false;
	}
	//m_pMap = new CMap();
	//if ( false == m_pMap->Init( pParser ) )
	//{
	//	assert_now( "Error parsing map" );
	//	pParser->Release();
	//	return false;
	//}

	CreateAndRegisterKey( "MouseLButton", m_pInputMgr->GetChangeSubject(), gs_DebugAddDisturbanceListener );
	CreateAndRegisterKey( "KeyF4", m_pInputMgr->GetDownSubject(), gs_SlowDownTimeObsvr );
	CreateAndRegisterKey( "KeyF5", m_pInputMgr->GetDownSubject(), gs_SpeedUpTimeObsvr );
	CreateAndRegisterKey( "KeyF6", m_pInputMgr->GetDownSubject(), gs_PauseGameListener );
#else // below is editor specific functionality:
	CreateAndRegisterKey( "KeyEscape", m_pInputMgr->GetDownSubject(), gs_EditorChangeToolListener );
	m_pEditorTool = IEditorTool::GetTool( ETool_SelectTool );
	m_pEditorTool->RegisterInput( m_pInputMgr );
#endif
	return true;
}

CMMMContext::~CMMMContext( void )
{
	if ( m_pFrameTimer )
	{
		delete m_pFrameTimer;
	}
	if ( m_pGameTimer )
	{
		delete m_pGameTimer;	
	}
	if ( m_pMouse )
	{
		delete m_pMouse;
	}
	if ( m_pWalls )
	{
		delete[] m_pWalls;
	}
	if ( m_pMap )
	{
		delete m_pMap;
	}
	if ( m_pAcorns )
	{
		delete[] m_pAcorns;
	}
	if ( m_pMoles )
	{
		delete[] m_pMoles;
	}
	if ( m_pTunnels )
	{
		delete[] m_pTunnels;
	}
#ifndef _EDITOR
	if ( m_pSquirrel )
	{
		delete m_pSquirrel;
	}
	if ( m_pDisturbanceLog )
	{
		delete[] m_pDisturbanceLog;
	}
#endif
}

void CMMMContext::ClearAllHighlights( void )
{
	for ( UINT32 i=0; i<m_uNumAcorns; i++ )
	{
		m_pAcorns[i].SetHighlighted( false );
	}
	for ( UINT32 i=0; i<m_uNumMoles; i++ )
	{
		m_pMoles[i].SetHighlighted( false );
	}
	for ( UINT32 i=0; i<m_uNumWalls; i++ )
	{
		m_pWalls[i].SetHighlighted( false );
	}
	for ( UINT32 i=0; i<m_uNumTunnels; i++ )
	{
		m_pTunnels[i].SetHighlighted( false );
	}
}

bool CMMMContext::HighlightBoxIntersections( const CBox2D& Box )
{
	for ( UINT32 i=0; i<m_uNumAcorns; i++ )
	{
		m_pAcorns[i].SetHighlighted( m_pAcorns[i].Intersects( Box ) );
	}
	for ( UINT32 i=0; i<m_uNumMoles; i++ )
	{
		m_pMoles[i].SetHighlighted( m_pMoles[i].Intersects( Box ) );
	}
	for ( UINT32 i=0; i<m_uNumWalls; i++ )
	{
		m_pWalls[i].SetHighlighted( m_pWalls[i].Intersects( Box ) );
	}
	for ( UINT32 i=0; i<m_uNumTunnels; i++ )
	{
		m_pTunnels[i].SetHighlighted( m_pTunnels[i].Intersects( Box ) );
	}
	return true;
}

#ifndef _EDITOR

bool CMMMContext::IsPaused( void ) const
{
	return m_pGameTimer->GetSpeedOfTime()==0.0f;
}
void CMMMContext::SetPaused( bool bPaused )
{
	m_pGameTimer->SetSpeedOfTime( bPaused?0.0f:m_fSpeedOfTime );
}

void CMMMContext::SetSpeedOfTime( float fVal )
{
	m_fSpeedOfTime = fVal;
	if ( false == IsPaused() )
	{
		m_pGameTimer->SetSpeedOfTime( m_fSpeedOfTime );
	}
}



void CMMMContext::UpdateAndRenderDisturbances( float fTimeDelta )
{
	float fAlpha;
	UINT32 uColour = 0xFFFF0000;
	for ( UINT32 i=0; i<m_uDisturbanceLogSize; i++ )
	{
		m_pDisturbanceLog[i].m_fAge += fTimeDelta;
		if ( m_fDisturbanceLogMaxAge > m_pDisturbanceLog[i].m_fAge )
		{
			fAlpha = 1.0f-(m_pDisturbanceLog[i].m_fAge/m_fDisturbanceLogMaxAge);
			uColour = 0x00FF0000;
			UINT32 uAlpha = UINT32(fAlpha*255.0f);
			UINT32 uCols = 255-uAlpha;
			uColour |= (uAlpha<<24)|(uCols<<8)|uCols;
//			DbgPrint( "Alpha %0.2f %08X\n", fAlpha, uColour );
			DrawCircle( m_pDisturbanceLog[i].m_Circle, uColour );
		}
	}
}



void CMMMContext::CreateDisturbance( CCircle& Disturbance )
{
	for ( UINT32 i=0; i<m_uDisturbanceLogSize; i++ )
	{
		if ( m_pDisturbanceLog[i].m_fAge >= m_fDisturbanceLogMaxAge )
		{
			m_pDisturbanceLog[i].m_fAge = 0.0f;
			m_pDisturbanceLog[i].m_Circle = Disturbance;
		}
	}

	for ( UINT32 i=0; i< m_uNumMoles; i++ )
	{
		m_pMoles[i].NotifyDisturbance( Disturbance );
	}
}

/**
 *	Load the given level number:
 */
bool CMMMContext::LoadLevel( UINT32 uLevelNum )
{
	if (m_bFirstRun)
	{
		m_iCurrentDifficulty = 0;
		m_bFirstRun = false;
	}
	else if (uLevelNum % m_uNumLevels == 0)
	{
		++ m_iCurrentDifficulty;
	}



	GDE::CDataParser* pParser = m_pParserManager->LoadResource( GDE::CResourceId( "MobMoleMadness.ini" ) );
	if ( NULL == pParser )
	{
		assert_now( "Unable to load/parse ini file" );
		pParser->Release();
		return false;
	}
	const static UINT32 uMAX_LEVEL_NAME_LEN = 128;
	char strLevelName[uMAX_LEVEL_NAME_LEN+1];
	char strSectionName[64];
	sprintf_s( strSectionName, "Level%d", uLevelNum%m_uNumLevels );
	if ( false == pParser->GetString( strLevelName, uMAX_LEVEL_NAME_LEN, strSectionName, "Name" ) )
	{
		assert_now( "Unable to find level name for %s", strSectionName );
		pParser->Release();
		return false;
	}
	pParser->Release();
	if ( Load( strLevelName ) )
	{
		m_uCurrentlLevel = uLevelNum;
		m_pSquirrel->Spawn();
		m_fPlayerDelay = m_fPlayerBaseDelay + (5 * m_iCurrentDifficulty);
		m_pSquirrel->ReleaseInput();


		if (m_iCurrentDifficulty > 0)
		{
			// Tell moles about the new difficuly
			for (UINT32 i = 0; i < m_uNumMoles; ++ i)
			{
				m_pMoles[i].UpdateDifficulty(m_iCurrentDifficulty);
			}
		}


		return true;
	}
	return false;
}

void	CMMMContext::DrawDelayMessage( void )
{
	const float fTOP_LEFT_X = 100.0f;
	const float fWIDTH = 600.0f;
	const float fDEPTH = 40.0f;
	const float fTOP_LEFT_Y = 200.0f;

	GDE::SVertex BoxVerts[4];
	BoxVerts[0].m_fX = fTOP_LEFT_X;
	BoxVerts[0].m_fY = fTOP_LEFT_Y;
	BoxVerts[1].m_fX = fTOP_LEFT_X+fWIDTH;
	BoxVerts[1].m_fY = fTOP_LEFT_Y;
	BoxVerts[2].m_fX = fTOP_LEFT_X+fWIDTH;
	BoxVerts[2].m_fY = fTOP_LEFT_Y+fDEPTH;
	BoxVerts[3].m_fX = fTOP_LEFT_X;
	BoxVerts[3].m_fY = fTOP_LEFT_Y+fDEPTH;

	for ( UINT32 i=0; i<4; i++ )
	{
		BoxVerts[i].m_fZ = 0.5f;
		BoxVerts[i].m_uColour = 0x3F000000;	// very gray
	}

	// untextured:
	CMMMContext::GetInstance().GetRenderer()->GetTextureManager().GetNULLTexture()->Apply( 0 );
	CMMMContext::GetInstance().GetRenderer()->DrawPrimitives( GDE::PT_QuadList, 1, BoxVerts );

	char InfoText[128];
	sprintf_s( InfoText, "Waiting to start: %0.1f seconds remain", m_fPlayerDelay );
	const UINT32 uOrigCol = m_pFont->GetColour();
	m_pFont->SetColour( 0xFF7F7FFF );
	m_pFont->Render( 290.0f, 212.0f, GDE::FJ_Left, InfoText );
	m_pFont->SetColour( uOrigCol );
}

void	CMMMContext::DrawGameOver( void )
{
	const float fTOP_LEFT_X = 100.0f;
	const float fWIDTH = 600.0f;
	const float fDEPTH = 40.0f;
	const float fTOP_LEFT_Y = 200.0f;

	GDE::SVertex BoxVerts[4];
	BoxVerts[0].m_fX = fTOP_LEFT_X;
	BoxVerts[0].m_fY = fTOP_LEFT_Y;
	BoxVerts[1].m_fX = fTOP_LEFT_X+fWIDTH;
	BoxVerts[1].m_fY = fTOP_LEFT_Y;
	BoxVerts[2].m_fX = fTOP_LEFT_X+fWIDTH;
	BoxVerts[2].m_fY = fTOP_LEFT_Y+fDEPTH;
	BoxVerts[3].m_fX = fTOP_LEFT_X;
	BoxVerts[3].m_fY = fTOP_LEFT_Y+fDEPTH;

	for ( UINT32 i=0; i<4; i++ )
	{
		BoxVerts[i].m_fZ = 0.5f;
		BoxVerts[i].m_uColour = 0x3F000000;	// very gray
	}

	// untextured:
	CMMMContext::GetInstance().GetRenderer()->GetTextureManager().GetNULLTexture()->Apply( 0 );
	CMMMContext::GetInstance().GetRenderer()->DrawPrimitives( GDE::PT_QuadList, 1, BoxVerts );

	m_pFont->Render( 320.0f, 212.0f, GDE::FJ_Left, "Game Over! Press Space" );
}

void CMMMContext::NotifySquirrelDeath( void )
{
	CreateAndRegisterKey( "KeySpace", m_pInputMgr->GetDownSubject(), gs_GameOverListener );
}

void CMMMContext::RestartGame( void )
{
	CreateAndRegisterKey( "KeySpace", m_pInputMgr->GetDownSubject(), gs_GameOverListener );
	m_pSquirrel->ResetLives();
	m_bFirstRun = true;
	m_iCurrentDifficulty = 0;
	LoadLevel( 0 );
}

#endif

void CMMMContext::CreateAndRegisterKey( const char* strKey, GDE::IInputChangeSubject& Subject, GDE::IObserver& Observer )
{
	// now register the observers
	GDE::UINT32 uCode;
	if ( m_pInputMgr->GetInputCode( strKey, uCode ) )
	{
		Subject.AddObserver( (GDE::IObserver*)&Observer, uCode );
	} else
	{
		assert_now( "Unable to bind input for key \"%s\"", strKey );
	}
}

void CMMMContext::RemoveKey( const char* strKey, GDE::IInputChangeSubject& Subject, GDE::IObserver& Observer )
{
	GDE::UINT32 uCode;
	if ( m_pInputMgr->GetInputCode( strKey, uCode ) )
	{
		Subject.RemoveObserver( (GDE::IObserver*)&Observer, uCode );
	} else
	{
		assert_now( "Unable to remove input for key \"%s\"", strKey );
	}
}



bool CMMMContext::FrameUpdate( void )
{
	m_pFrameTimer->FrameUpdate();
	m_pGameTimer->FrameUpdate();
	float fTimeInterval = m_pGameTimer->GetFrameInterval(); // allows pausing.


	if (fTimeInterval > 1.0f / 60.0f)
	{
		fTimeInterval = 1.0f / 60.0f;
	}


#ifndef _EDITOR
	if ( m_fPlayerDelay > 0.0f )
	{
		if ( m_fPlayerDelay < fTimeInterval )
		{
			m_pSquirrel->RegisterInput();
		}
		m_fPlayerDelay -= fTimeInterval;
	}
#endif

	m_pInputMgr->Update( fTimeInterval );

	// do all required updates here.
#ifndef _EDITOR


	for ( UINT32 i=0; i<m_uNumAcorns; i++ )
	{
		if ( m_pAcorns[i].GetState() == CAcorn::ES_Available)
		{
			m_pAcorns[i].Update(m_pMoles, m_uNumMoles);
		}
	}


	for ( UINT32 i=0; i<m_uNumMoles ; i++ )
	{
		m_pMoles[i].Update( fTimeInterval, *m_pSquirrel );
	}
	m_pSquirrel->Update( fTimeInterval );
	bool bAcornsRemain = false;
	for ( UINT32 i=0; i<m_uNumAcorns; i++ )
	{
		if ( m_pAcorns[i].GetState() != CAcorn::ES_Collected )
		{
			bAcornsRemain = true;
			break;
		}
	}
	if ( false == bAcornsRemain )
	{
		LoadLevel( m_uCurrentlLevel+1 );
	}
#endif


//	CGrid* pGrid = m_pMap->GetGridOnScreen( m_pMouse->GetPosition() );

	// now for all the rendering,
	m_pRenderer->BeginScene();
	m_pRenderer->ColourClear( 0xFFFFFFFF );

	for ( UINT32 i=0; i<m_uNumWalls; i++ )
	{
		m_pWalls[i].Draw();
	}

	for ( UINT32 i=0; i<m_uNumTunnels; i++ )
	{
		m_pTunnels[i].Draw();
	}

	for ( UINT32 i=0; i<m_uNumAcorns; i++ )
	{
		m_pAcorns[i].Draw();
	}


	// Draw moles path finding
#ifndef _EDITOR
	const CPath* kpPath = CMole::GetSharedPath();
	kpPath->Draw();

#else

	CPath* pPath = CMole::GetPath();


	if (pPath != 0)
	{
		pPath->Draw();
	}

#endif


	for ( UINT32 i=0; i<m_uNumMoles; i++ )
	{
		if (i == 0)
		{
			m_pMoles[i].Draw(true);
		}
		else
		{
			m_pMoles[i].Draw();
		}
	}

	//m_pMap->Draw();

	//if ( pGrid )
	//{
	//	const float fVal = m_pFrameTimer->GetCyclicalOverTime( 1.0f );
	//	pGrid->DrawCross( (fVal>0.5f)?0xFF00FF00:0xFFFF0000 );
	//	pGrid->DrawOutline( (fVal<=0.5f)?0xFF00FF00:0xFFFF0000 );

	//	//CGrid* pSourceGrid = m_pMap->GetGrid( 0, 0 );
	//	//if ( pSourceGrid != pGrid )
	//	//{
	//	//	pSourceGrid->DrawArrowTo( pGrid, 0xFF0000FF, fVal );
	//	//}

	//	for ( UINT32 i=0; i<pGrid->GetMovableNeighbourCount(); i++ )
	//	{
	//		pGrid->DrawArrowTo(	pGrid->GetMovableNeighbour( i ), 0xFF0000FF, 
	//			m_pFrameTimer->GetCyclicalOverTime( (float)pGrid->GetMovableNeighbour( i )->GetCost() ) );
	//	}
	//}



#ifdef _EDITOR
	m_pEditorTool->Render();
#else
	m_pSquirrel->Draw();
	if ( m_pSquirrel->IsDead() )
	{
		DrawGameOver();
	}
	gs_DebugAddDisturbanceListener.UpdateAndRender( fTimeInterval );
	UpdateAndRenderDisturbances( fTimeInterval );
	if ( m_fPlayerDelay > 0.0f )
	{
		DrawDelayMessage();
	}
#endif

	m_pMouse->Draw();

	const float fTOP_LEFT_X = 00.0f;
	const float fWIDTH = 800.0f;
	const float fDEPTH = 20.0f;
	const float fTOP_LEFT_Y = 600.0f-fDEPTH;

	GDE::SVertex BoxVerts[4];
	BoxVerts[0].m_fX = fTOP_LEFT_X;
	BoxVerts[0].m_fY = fTOP_LEFT_Y;
	BoxVerts[1].m_fX = fTOP_LEFT_X+fWIDTH;
	BoxVerts[1].m_fY = fTOP_LEFT_Y;
	BoxVerts[2].m_fX = fTOP_LEFT_X+fWIDTH;
	BoxVerts[2].m_fY = fTOP_LEFT_Y+fDEPTH;
	BoxVerts[3].m_fX = fTOP_LEFT_X;
	BoxVerts[3].m_fY = fTOP_LEFT_Y+fDEPTH;

	for ( UINT32 i=0; i<4; i++ )
	{
		BoxVerts[i].m_fZ = 0.5f;
		BoxVerts[i].m_uColour = 0x3F000000;	// very gray
	}

	// untextured:
	CMMMContext::GetInstance().GetRenderer()->GetTextureManager().GetNULLTexture()->Apply( 0 );
	CMMMContext::GetInstance().GetRenderer()->DrawPrimitives( GDE::PT_QuadList, 1, BoxVerts );


//	if ( m_bDrawInfoText )
	{
		char InfoText[256];
		//if ( pGrid )
		//{
		//	SGridRef GridRef;
		//	m_pMap->GetGridRef( pGrid, GridRef );
		//	sprintf_s( InfoText, "Selected grid [%d,%d] %s, Cost %d",
		//		GridRef.m_iX, GridRef.m_iY, pGrid->GetName(), pGrid->GetCost() );
		//	m_pFont->Render( 10, 560, GDE::FJ_Left, InfoText );
		//}

#ifdef _EDITOR
		m_pFont->Render( 160, 583, GDE::FJ_Left, m_pEditorTool->GetDescription() );
#else
		sprintf_s( InfoText, "Level %d  Lives %d  Score %02d  Acorns %d Speed %0.2f%%%s", m_uCurrentlLevel, m_pSquirrel->GetLives(), m_pSquirrel->GetScore(), m_pSquirrel->GetAcornCount(), m_fSpeedOfTime*100.0f, IsPaused()?" PAUSED":"" );
		m_pFont->Render( 160, 583, GDE::FJ_Left, InfoText );
#endif

		const float fFrameRate = 1.0f/m_pFrameTimer->GetFrameInterval();
		const UINT32 uFrameRate = UINT32(fFrameRate);
		const UINT32 uFrameDecimal = UINT32(fFrameRate*100.0f)%100;
		sprintf_s( InfoText, "Framerate %05d.%02d", uFrameRate, uFrameDecimal );
		m_pFont->Render( 10, 583, GDE::FJ_Left, InfoText );
	}

	m_pRenderer->EndScene();
	return true;
}

void CMMMContext::ReleaseResources( void )
{
	m_pFont->Release();
	if ( m_pMap )
	{
		delete m_pMap;
		m_pMap = NULL;
	}
	if ( m_pAcorns )
	{
		delete[] m_pAcorns;
		m_pAcorns = NULL;
	}
	if ( m_pMoles )
	{
		delete[] m_pMoles;
		m_pMoles = NULL;
	}
	if ( m_pTunnels )
	{
		delete[] m_pTunnels;
		m_pTunnels = NULL;
	}
#ifndef _EDITOR
	if ( m_pSquirrel )
	{
		delete m_pSquirrel;
		m_pSquirrel = NULL;
	}
#endif
}

/**
 *	@param Circle	The circle to draw.
 *	@param uColour	The colour to draw with.
 */
void CMMMContext::DrawCircle( const CCircle& Circle, UINT32 uColour )
{
	// how many line segments?
	const float fRadius = Circle.Radius();
	const float fCircumference = 2.0f * fPI * fRadius;
	
	const static UINT32 uMAX_SEGMENTS = 20;
	UINT32 uSegments = UINT32( fCircumference*0.2f );
	if ( uSegments>uMAX_SEGMENTS )
	{
		uSegments = uMAX_SEGMENTS;
	}

	// TODO: optimise with line strip rather than list.
	GDE::SVertex Verts[uMAX_SEGMENTS*2+2];
	const float fAngleStep = 2.0f*fPI/uSegments;
	float fCurAngle;
	Verts[0].m_fX = fRadius + Circle.Centre().X();
	Verts[0].m_fY = Circle.Centre().Y();;
	Verts[0].m_fZ = 0.5f;
	Verts[0].m_uColour = uColour;
	Verts[uSegments*2-1] = Verts[0];
	for ( UINT32 i=1; i<uSegments;i++ )
	{
		fCurAngle = fAngleStep * i;
		Verts[i*2-1].m_fX = cosf( fCurAngle ) * fRadius + Circle.Centre().X();
		Verts[i*2-1].m_fY = sinf( fCurAngle ) * fRadius + Circle.Centre().Y();
		Verts[i*2-1].m_fZ = 0.5f;
		Verts[i*2-1].m_uColour = uColour;
		Verts[i*2] = Verts[i*2-1];
	}

	// untextured:
	m_pRenderer->GetTextureManager().GetNULLTexture()->Apply( 0 );
	m_pRenderer->DrawPrimitives( GDE::PT_LineList, uSegments, Verts );
}


/**
 *	@param Circle	The circle to draw.
 *	@param uColour	The colour to draw with.
 */
void CMMMContext::DrawFilledCircle( const CCircle& Circle, UINT32 uColour )
{
	// how many line segments?
	const static float fPI = 3.141592653589793f;
	const float fRadius = Circle.Radius();
	const float fCircumference = 2.0f * fPI * fRadius;
	
	const static UINT32 uMAX_SEGMENTS = 20;
	UINT32 uSegments = UINT32( fCircumference*0.2f );
	if ( uSegments>uMAX_SEGMENTS )
	{
		uSegments = uMAX_SEGMENTS;
	}
	if ( uSegments < 3 )
	{
		uSegments=3;
	}

	// TODO: optimise with line strip rather than list.
	GDE::SVertex Verts[uMAX_SEGMENTS*3];
	const float fAngleStep = 2.0f*fPI/uSegments;
	float fCurAngle;
	Verts[0].m_fX = Circle.Centre().X();
	Verts[0].m_fY = Circle.Centre().Y();;
	Verts[0].m_fZ = 0.5f;
	Verts[0].m_uColour = uColour;
	Verts[1].m_fX = fRadius + Circle.Centre().X();
	Verts[1].m_fY = Circle.Centre().Y();;
	Verts[1].m_fZ = 0.5f;
	Verts[1].m_uColour = uColour;
	Verts[uSegments*3-1] = Verts[1];
	for ( UINT32 i=1; i<uSegments;i++ )
	{
		fCurAngle = fAngleStep * i;
		Verts[i*3-1].m_fX = cosf( fCurAngle ) * fRadius + Circle.Centre().X();
		Verts[i*3-1].m_fY = sinf( fCurAngle ) * fRadius + Circle.Centre().Y();
		Verts[i*3-1].m_fZ = 0.5f;
		Verts[i*3-1].m_uColour = uColour;
		Verts[i*3+1] = Verts[i*3-1];
		Verts[i*3] = Verts[0];
	}
	// untextured:
	m_pRenderer->GetTextureManager().GetNULLTexture()->Apply( 0 );
	m_pRenderer->DrawPrimitives( GDE::PT_TriangleList, uSegments, Verts );
}

void CMMMContext::DrawLine(const CFVec2& v0, const CFVec2& v1, GDE::UINT32 uColour)
{
	GDE::SVertex Verts[2];
	Verts[0].m_fX = v0.X();
	Verts[0].m_fY = v0.Y();
	Verts[0].m_fZ = 0.5f;
	Verts[0].m_uColour = uColour;

	Verts[1].m_fX = v1.X();
	Verts[1].m_fY = v1.Y();
	Verts[1].m_fZ = 0.5f;
	Verts[1].m_uColour = uColour;

	// untextured:
	m_pRenderer->GetTextureManager().GetNULLTexture()->Apply( 0 );
	m_pRenderer->DrawPrimitives( GDE::PT_LineList, 1, Verts );
}

void  CMMMContext::DrawBox( const CBox2D& Box, UINT32 uColour )
{
	GDE::SVertex Verts[8];
	Verts[0].m_fX = Box.GetCentre().X() - Box.GetExtents().X();	
	Verts[0].m_fY = Box.GetCentre().Y() - Box.GetExtents().Y();	
	Verts[7].m_fX = Box.GetCentre().X() - Box.GetExtents().X();	
	Verts[7].m_fY = Box.GetCentre().Y() - Box.GetExtents().Y();	

	Verts[1].m_fX = Box.GetCentre().X() + Box.GetExtents().X();	
	Verts[1].m_fY = Box.GetCentre().Y() - Box.GetExtents().Y();	
	Verts[2].m_fX = Box.GetCentre().X() + Box.GetExtents().X();	
	Verts[2].m_fY = Box.GetCentre().Y() - Box.GetExtents().Y();	

	Verts[3].m_fX = Box.GetCentre().X() + Box.GetExtents().X();	
	Verts[3].m_fY = Box.GetCentre().Y() + Box.GetExtents().Y();	
	Verts[4].m_fX = Box.GetCentre().X() + Box.GetExtents().X();	
	Verts[4].m_fY = Box.GetCentre().Y() + Box.GetExtents().Y();	

	Verts[5].m_fX = Box.GetCentre().X() - Box.GetExtents().X();	
	Verts[5].m_fY = Box.GetCentre().Y() + Box.GetExtents().Y();	
	Verts[6].m_fX = Box.GetCentre().X() - Box.GetExtents().X();	
	Verts[6].m_fY = Box.GetCentre().Y() + Box.GetExtents().Y();	
	for ( UINT32 i=0; i<8;i++ )
	{
		Verts[i].m_uColour = uColour;
		Verts[i].m_fZ = 0.5f;
	}
	// untextured:
	m_pRenderer->GetTextureManager().GetNULLTexture()->Apply( 0 );
	m_pRenderer->DrawPrimitives( GDE::PT_LineList, 4, Verts );

}

/**
 *	Does the given circle intersect anything?
 */
bool CMMMContext::CircleIntersectsAnything( const CCircle& Circle )
{
	for ( UINT32 i=0; i<m_uNumWalls; i++ )
	{
		if ( m_pWalls[i].Intersects( Circle ) )
		{
			return true;
		}
	}
	for ( UINT32 i=0; i<m_uNumAcorns; i++ )
	{
		if ( m_pAcorns[i].Intersects( Circle ) )
		{
			return true;
		}
	}
	for ( UINT32 i=0; i<m_uNumMoles; i++ )
	{
		if ( m_pMoles[i].Intersects( Circle ) )
		{
			return true;
		}
	}
	for ( UINT32 i=0; i<m_uNumTunnels; i++ )
	{
		if ( m_pTunnels[i].Intersects( Circle ) )
		{
			return true;
		}
	}
	
	return false;
}

/**
 *	Does the given circle intersect any solid geometry - i.e a collision has occured.
 */
bool CMMMContext::CircleIntersectsGeometry( const CCircle& Circle )
{
	for ( UINT32 i=0; i<m_uNumWalls; i++ )
	{
		if ( m_pWalls[i].Intersects( Circle ) )
		{
			return true;
		}
	}
	return false;
}

/**
 *	@param pAcorns		Filled in with pointer to acorn array.
 *	@param uNumAcorns	Filled in with the number of acorns.
 */
void CMMMContext::GetAcorns( CAcorn*& pAcorns, UINT32& uNumAcorns )
{
	pAcorns = m_pAcorns;
	uNumAcorns = m_uNumAcorns;
}

/**
 *	@param pAcorns		Filled in with pointer to tunnel array.
 *	@param uNumAcorns	Filled in with the number of tunnel.
 */
void CMMMContext::GetTunnels( CTunnel*& pTunnels, UINT32& uNumTunnels )
{
	pTunnels = m_pTunnels;
	uNumTunnels = m_uNumTunnels;
}

bool CMMMContext::Load( const char* strFileName )
{
	// now load in the ini file:
	DbgPrint( "Loading level \"%s\"\n", strFileName );

	GDE::CDataParser* pParser = m_pParserManager->LoadResource( GDE::CResourceId( strFileName ) );
	if ( NULL == pParser )
	{
		assert_now( "Unable to load/parse ini file" );
		return false;
	}

	// load walls:
	UINT32 uWallCount;
	if ( false == pParser->GetValue( uWallCount, "Main", "Walls" ) )
	{
		assert_now( "INI: no wall count" );
		pParser->Release();
		return false;
	}

	CWall* pWalls = new CWall[uWallCount];
	TScopeDeleter<CWall> WallDeleter( pWalls );
	float fX0, fY0, fX1, fY1;
	char strItemName[10];
	for ( UINT32 i=0; i<uWallCount; i++ )
	{
		sprintf_s( strItemName, "Wall%d", i );
		if ( false == pParser->GetValue( fX0, strItemName, "X0" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "X0" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fY0, strItemName, "Y0" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "Y0" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fX1, strItemName, "X1" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "X1" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fY1, strItemName, "Y1" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "Y1" );
			pParser->Release();
			return false;
		}

		pWalls[i].Initialise( SFVec2( fX0, fY0 ), SFVec2( fX1, fY1 ) );
	}

	// load acorns:
	UINT32 uAcornCount;
	if ( false == pParser->GetValue( uAcornCount, "Main", "Acorns" ) )
	{
		assert_now( "INI: no Acorn count" );
		pParser->Release();
		return false;
	}

	CAcorn* pAcorns = new CAcorn[uAcornCount];
	TScopeDeleter<CAcorn> AcornDeleter( pAcorns );
	for ( UINT32 i=0; i<uAcornCount; i++ )
	{
		sprintf_s( strItemName, "Acorn%d", i );
		if ( false == pParser->GetValue( fX0, strItemName, "X" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "X" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fY0, strItemName, "Y" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "Y" );
			pParser->Release();
			return false;
		}

		pAcorns[i].Init( SFVec2( fX0, fY0 ), "Acorn.png" );
	}

	// load tunnels:
	UINT32 uTunnelCount;
	if ( false == pParser->GetValue( uTunnelCount, "Main", "Tunnels" ) )
	{
		assert_now( "INI: no Tunnel count" );
		pParser->Release();
		return false;
	}

	CTunnel* pTunnels = new CTunnel[uTunnelCount];
	TScopeDeleter<CTunnel> TunnelDeleter( pTunnels );
	for ( UINT32 i=0; i<uTunnelCount; i++ )
	{
		sprintf_s( strItemName, "Tunnel%d", i );
		if ( false == pParser->GetValue( fX0, strItemName, "X" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "X" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fY0, strItemName, "Y" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "Y" );
			pParser->Release();
			return false;
		}

		pTunnels[i].Init( SFVec2( fX0, fY0 ), "Tunnel.png" );
	}

	// load moles:
	UINT32 uMoleCount;
	if ( false == pParser->GetValue( uMoleCount, "Main", "Moles" ) )
	{
		assert_now( "INI: no Mole count" );
		pParser->Release();
		return false;
	}


	// all loaded OK
	if ( m_pWalls )
	{
		delete[] m_pWalls;
	}
	m_pWalls = pWalls;
	WallDeleter.Release();
	m_uNumWalls = uWallCount;



	delete[] m_pMoles;
	m_pMoles = 0;

	m_pMoles = new CMole[uMoleCount];
	for ( UINT32 i=0; i<uMoleCount; i++ )
	{
		sprintf_s( strItemName, "Mole%d", i );
		if ( false == pParser->GetValue( fX0, strItemName, "X" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "X" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fY0, strItemName, "Y" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "Y" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->GetValue( fX1, strItemName, "Heading" ) )
		{
			assert_now( "Unable to find ini value %s:%s", strItemName, "Heading" );
			pParser->Release();
			return false;
		}

		m_pMoles[i].Initialise( SFVec2( fX0, fY0 ), "Mole.png", fX1 );
	}


	pParser->Release();


	
	if ( m_pAcorns )
	{
		delete[] m_pAcorns;
	}
	m_pAcorns = pAcorns;
	AcornDeleter.Release();
	m_uNumAcorns = uAcornCount;

	if ( m_pTunnels )
	{
		delete[] m_pTunnels;
	}
	m_pTunnels = pTunnels;
	TunnelDeleter.Release();
	m_uNumTunnels = uTunnelCount;
	m_uNumMoles = uMoleCount;

	DbgPrint( "Load \"%s\" completed OK\n", strFileName );
	return true;
}



#ifdef _EDITOR
bool CMMMContext::Save( const char* strFileName )
{
	DbgPrint( "Save as \"%s\"\n", strFileName );
	GDE::CDataParser* pParser = m_pParserManager->LoadResource( GDE::CResourceId( strFileName ) );
	if ( NULL == pParser )
	{
		assert_now( "Unable to create/writee ini file" );
		return false;
	}

	// save walls:
	if ( false == pParser->SetValue( m_uNumWalls, "Main", "Walls" ) )
	{
		assert_now( "INI: write wall count" );
		pParser->Release();
		return false;
	}

	char strItemName[10];
	for ( UINT32 i=0; i<m_uNumWalls; i++ )
	{
		sprintf_s( strItemName, "Wall%d", i );
		if ( false == pParser->SetValue( m_pWalls[i].GetEnd0().X(), strItemName, "X0" ) )
		{
			assert_now( "Unable to set ini value %s:%s", strItemName, "X0" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->SetValue(  m_pWalls[i].GetEnd0().Y(), strItemName, "Y0" ) )
		{
			assert_now( "Unable to set ini value %s:%s", strItemName, "Y0" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->SetValue(  m_pWalls[i].GetEnd1().X(), strItemName, "X1" ) )
		{
			assert_now( "Unable to set ini value %s:%s", strItemName, "X1" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->SetValue(  m_pWalls[i].GetEnd1().Y(), strItemName, "Y1" ) )
		{
			assert_now( "Unable to set ini value %s:%s", strItemName, "Y1" );
			pParser->Release();
			return false;
		}
	}

	// save acorns:
	if ( false == pParser->SetValue( m_uNumAcorns, "Main", "Acorns" ) )
	{
		assert_now( "INI: write Acorn count" );
		pParser->Release();
		return false;
	}

	for ( UINT32 i=0; i<m_uNumAcorns; i++ )
	{
		sprintf_s( strItemName, "Acorn%d", i );
		if ( false == pParser->SetValue( m_pAcorns[i].GetPosition().X(), strItemName, "X" ) )
		{
			assert_now( "Unable to set ini value %s:%s", strItemName, "X" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->SetValue( m_pAcorns[i].GetPosition().Y(), strItemName, "Y" ) )
		{
			assert_now( "Unable to set ini value %s:%s", strItemName, "Y" );
			pParser->Release();
			return false;
		}
	}

	// save tunnels:
	if ( false == pParser->SetValue( m_uNumTunnels, "Main", "Tunnels" ) )
	{
		assert_now( "INI: write Tunnel count" );
		pParser->Release();
		return false;
	}

	for ( UINT32 i=0; i<m_uNumTunnels; i++ )
	{
		sprintf_s( strItemName, "Tunnel%d", i );
		if ( false == pParser->SetValue( m_pTunnels[i].GetPosition().X(), strItemName, "X" ) )
		{
			assert_now( "Unable to set ini value %s:%s", strItemName, "X" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->SetValue( m_pTunnels[i].GetPosition().Y(), strItemName, "Y" ) )
		{
			assert_now( "Unable to set ini value %s:%s", strItemName, "Y" );
			pParser->Release();
			return false;
		}
	}

	// save moles:
	if ( false == pParser->SetValue( m_uNumMoles, "Main", "Moles" ) )
	{
		assert_now( "INI: Write Mole count" );
		pParser->Release();
		return false;
	}

	for ( UINT32 i=0; i<m_uNumMoles; i++ )
	{
		sprintf_s( strItemName, "Mole%d", i );
		if ( false == pParser->SetValue( m_pMoles[i].GetPosition().X(), strItemName, "X" ) )
		{
			assert_now( "Unable to set ini value %s:%s", strItemName, "X" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->SetValue( m_pMoles[i].GetPosition().Y(), strItemName, "Y" ) )
		{
			assert_now( "Unable to set ini value %s:%s", strItemName, "Y" );
			pParser->Release();
			return false;
		}
		if ( false == pParser->SetValue( m_pMoles[i].GetHeading(), strItemName, "Heading" ) )
		{
			assert_now( "Unable to set ini value %s:%s", strItemName, "Heading" );
			pParser->Release();
			return false;
		}
	}

	pParser->Release();

	DbgPrint( "Save \"%s\" completed OK\n", strFileName );
	return true;
}


void CMMMContext::SetEditTool( IEditorTool* pTool )
{
	if ( m_pEditorTool )
	{
		m_pEditorTool->ReleaseInput( m_pInputMgr );
	}
	m_pEditorTool = pTool;
	if (m_pEditorTool )
	{
		m_pEditorTool->RegisterInput( m_pInputMgr );
	}
}


bool CMMMContext::CreateWalls(const CFVec2 *pPositions, GDE::UINT32 uWallCount)
{
//	UNUSED_VAR( pPositions );
//	UNUSED_VAR( uWallCount );
	// Add the walls to the ini file:
	const UINT32 uNewNumWalls = m_uNumWalls+uWallCount;
	CWall* pNewWalls = new CWall[uNewNumWalls];
	for ( UINT32 i=0; i<m_uNumWalls; i++ )
	{
		pNewWalls[i].Initialise( m_pWalls[i].GetEnd0(), m_pWalls[i].GetEnd1() );
	}
	while ( m_uNumWalls < uNewNumWalls )
	{
		pNewWalls[m_uNumWalls++].Initialise( pPositions[0], pPositions[1] );
		pPositions+=2;
	}
	if ( m_pWalls )
	{
		delete[] m_pWalls;
	}
	m_pWalls = pNewWalls;
	return true;
}

bool CMMMContext::CreateAcorn( const CFVec2& vPosition )
{
	CCircle Bounds;
	Bounds.Initialise( vPosition, fACORN_RADIUS );
	if ( CircleIntersectsAnything( Bounds ) )
	{
		return false;
	}
	const UINT32 uNewAcorns = m_uNumAcorns + 1;
	CAcorn* pNewAcorns = new CAcorn[uNewAcorns];
	for ( UINT32 i=0; i< m_uNumAcorns; i++ )
	{
		pNewAcorns[i].Init( m_pAcorns[i].GetPosition(), "Acorn.png" );
	}
	pNewAcorns[m_uNumAcorns++].Init( vPosition, "Acorn.png" );
	delete[] m_pAcorns;
	m_pAcorns = pNewAcorns;
	return true;
}

bool CMMMContext::CreateTunnel( const CFVec2& vPosition )
{
	CCircle Bounds;
	Bounds.Initialise( vPosition, fACORN_RADIUS );
	if ( CircleIntersectsAnything( Bounds ) )
	{
		return false;
	}
	const UINT32 uNewTunnels = m_uNumTunnels + 1;
	CTunnel* pNewTunnels = new CTunnel[uNewTunnels];
	for ( UINT32 i=0; i< m_uNumTunnels; i++ )
	{
		pNewTunnels[i].Init( m_pTunnels[i].GetPosition(), "Tunnel.png" );
	}
	pNewTunnels[m_uNumTunnels++].Init( vPosition, "Tunnel.png" );
	delete[] m_pTunnels;
	m_pTunnels = pNewTunnels;
	return true;
}


bool CMMMContext::CreateMole( const CFVec2& vPosition, float fHeading )
{
	CCircle Bounds;
	Bounds.Initialise( vPosition, fMOLE_RADIUS );
	if ( CircleIntersectsAnything( Bounds ) )
	{
		return false;
	}
	const UINT32 uNewMoles = m_uNumMoles + 1;
	CMole* pNewMoles = new CMole[uNewMoles];
	for ( UINT32 i=0; i< m_uNumMoles; i++ )
	{
		pNewMoles[i].Initialise( m_pMoles[i].GetPosition(), "Mole.png", m_pMoles[i].GetHeading() );
	}
	pNewMoles[m_uNumMoles++].Initialise( vPosition, "Mole.png", fHeading );
	delete[] m_pMoles;
	m_pMoles = pNewMoles;
	return true;
	
}

void CMMMContext::DeleteHighlighedItems( void )
{
	UINT32 uFinalCount = 0;
	for ( UINT32 i=0; i<m_uNumAcorns; i++ )
	{
		if ( false == m_pAcorns[i].IsHighlighted() )
		{
			uFinalCount++;
		}
	}
	if ( uFinalCount != m_uNumAcorns )
	{
		CAcorn* pAcorns = new CAcorn[uFinalCount];
		uFinalCount = 0;
		for ( UINT32 i=0; i<m_uNumAcorns; i++ )
		{
			if ( false == m_pAcorns[i].IsHighlighted() )
			{
				pAcorns[uFinalCount++].Init( m_pAcorns[i].GetPosition(), "Acorn.png" );
			}
		}
		m_uNumAcorns = uFinalCount;
		delete[] m_pAcorns;
		m_pAcorns = pAcorns;
	}

	uFinalCount = 0;
	for ( UINT32 i=0; i<m_uNumMoles; i++ )
	{
		if ( false == m_pMoles[i].IsHighlighted() )
		{
			uFinalCount++;
		}
	}
	if ( uFinalCount != m_uNumMoles )
	{
		CMole* pMoles = new CMole[uFinalCount];
		uFinalCount = 0;
		for ( UINT32 i=0; i<m_uNumMoles; i++ )
		{
			if ( false == m_pMoles[i].IsHighlighted() )
			{
				pMoles[uFinalCount++].Initialise( m_pMoles[i].GetPosition(), "Mole.png", m_pMoles[i].GetHeading() );
			}
		}
		m_uNumMoles = uFinalCount;
		delete[] m_pMoles;
		m_pMoles = pMoles;
	}

	uFinalCount = 0;
	for ( UINT32 i=0; i<m_uNumWalls; i++ )
	{
		if ( false == m_pWalls[i].IsHighlighted() )
		{
			uFinalCount++;
		}
	}
	if ( uFinalCount != m_uNumWalls )
	{
		CWall* pWalls = new CWall[uFinalCount];
		uFinalCount = 0;
		for ( UINT32 i=0; i<m_uNumWalls; i++ )
		{
			if ( false == m_pWalls[i].IsHighlighted() )
			{
				pWalls[uFinalCount++].Initialise( m_pWalls[i].GetEnd0(),  m_pWalls[i].GetEnd1() );
			}
		}
		m_uNumWalls = uFinalCount;
		delete[] m_pWalls;
		m_pWalls = pWalls;
	}

	
	uFinalCount = 0;
	for ( UINT32 i=0; i<m_uNumTunnels; i++ )
	{
		if ( false == m_pTunnels[i].IsHighlighted() )
		{
			uFinalCount++;
		}
	}
	if ( uFinalCount != m_uNumTunnels )
	{
		CTunnel* pTunnels = new CTunnel[uFinalCount];
		uFinalCount = 0;
		for ( UINT32 i=0; i<m_uNumTunnels; i++ )
		{
			if ( false == m_pTunnels[i].IsHighlighted() )
			{
				pTunnels[uFinalCount++].Init( m_pTunnels[i].GetPosition(), "Tunnel.png" );
			}
		}
		m_uNumTunnels = uFinalCount;
		delete[] m_pTunnels;
		m_pTunnels = pTunnels;
	}

	
}


















#endif // ifdef _EDITOR








#ifndef _EDITOR

void
CAddLive::NotifyChange( GDE::ISubject* pSubject, UINT32 uMessage )
{
	pSubject;
	uMessage;
	CMMMContext::GetInstance().GetSquirrel()->IncrmentLives();
}


void
CSkipLevel::NotifyChange( GDE::ISubject* pSubject, UINT32 uMessage )
{
	pSubject;
	uMessage;


	//UINT32 uiNumLevels = CMMMContext::GetInstance().GetNumLevels();
	UINT32 uiCurrentLevelId = CMMMContext::GetInstance().GetCurrentLevelId();


	CMMMContext::GetInstance().LoadLevel(uiCurrentLevelId + 1);
}


void
CSleepMoles::NotifyChange( GDE::ISubject* pSubject, UINT32 uMessage )
{
	pSubject;
	uMessage;
	CMole* pMoles = 0;
	int iNumMoles = 0;


	CMMMContext::GetInstance().GetMoles(pMoles, iNumMoles);


	for (int i = 0; i < iNumMoles; ++ i)
	{
		pMoles[i].AppendSnoozeTask(5.0f);
	}
}


#endif


