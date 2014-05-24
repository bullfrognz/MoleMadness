/**
 *	@file MobMoleMadness\MMMEditorContext.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _MobMoleMadnessCONTEXT_H__Included_214923253
#define _MobMoleMadnessCONTEXT_H__Included_214923253

#include "GDE/Renderer/RendererContext.h"

#include "FVector2.h"

#ifdef _EDITOR
# include "EditorTool.h"
#endif

// forward reference GDE members:
namespace GDE
{
	class CClock;
	class CRenderer;
	class CInputDevice;
	class IInputChangeSubject;
	class IObserver;
	class CDataParserManager;
	class CFontManager;
	class CFont;
	class CSpriteManager;
};

// forward reference MobMoleMadness members:
class CMouse;
class CCircle;
class CMap;
class CWall;
class CAcorn;
class CMole;
class CTunnel;
class CBox2D;
#ifndef _EDITOR
class CSquirrel;
#endif

const static float fACORN_RADIUS = 18.0f; //!< radius of acorns, to avoid them intersecting.
const static float fMOLE_RADIUS = 13.0f;	//!< radius of moles.
const static float fTUNNEL_RADIUS = 13.0f; //!< radius of tunnels.








#ifndef _EDITOR

#include "GDE/Core/Observer.h"

class CAddLive : public GDE::IObserver
{
public:
	virtual void NotifyChange( GDE::ISubject* pSubject, UINT32 uMessage );
};
class CSkipLevel : public GDE::IObserver
{
public:
	virtual void NotifyChange( GDE::ISubject* pSubject, UINT32 uMessage );
};
class CSleepMoles : public GDE::IObserver
{
public:
	virtual void NotifyChange( GDE::ISubject* pSubject, UINT32 uMessage );
};
#endif








/**
 *	The MobMoleMadness context, defining the application.
 */
class CMMMContext : public GDE::IRendererContext
{
protected:
	CMMMContext();	// it's a singleton..
public:
	static CMMMContext& GetInstance( void );
	~CMMMContext();
	bool					Initialise( 
								GDE::CRenderer& Renderer, 
								GDE::CInputDevice& InputMgr, 
								GDE::CFontManager& FontManager,
								GDE::CDataParserManager& DataParserManager,
								GDE::CSpriteManager& SpriteManager );
	void					ReleaseResources( void );
	void					CreateAndRegisterKey( const char* strKey, GDE::IInputChangeSubject& Subject, GDE::IObserver& Observer );
	void					RemoveKey( const char* strKey, GDE::IInputChangeSubject& Subject, GDE::IObserver& Observer );

	virtual bool			FrameUpdate( void );

	GDE::CRenderer*			GetRenderer( void ) { return m_pRenderer; }
	GDE::CSpriteManager*	GetSpriteM( void ) { return m_pSpriteManager; }
	GDE::CInputDevice*		GetInput( void ) { return m_pInputMgr; }
	void					DrawCircle( const CCircle& Circle, UINT32 uColour );
	void					DrawFilledCircle( const CCircle& Circle, UINT32 uColour );
	void					DrawLine( const CFVec2& v0, const CFVec2& v1, UINT32 uColour );
	void					DrawBox( const CBox2D& Box, UINT32 uColour );
	CMap*					GetMap( void ) { return m_pMap; }
	CMouse*					GetMouse( void ) { return m_pMouse; }
	bool					CircleIntersectsAnything( const CCircle& Circle );
	bool					CircleIntersectsGeometry( const CCircle& Circle );
	void					ClearAllHighlights( void );
	bool					HighlightBoxIntersections( const CBox2D& Box );
	GDE::CFont*				GetFont( void ) { return m_pFont; }
	const GDE::CClock*		GetGameTimer( void ) { return m_pGameTimer; }
	bool					Load( const char* strFileName );
	void					GetAcorns( CAcorn*& pAcorns, UINT32& uNumAcorns );
	void					GetTunnels( CTunnel*& pTunnels, UINT32& uNumTunnels );
#ifndef _EDITOR
	float					GetSpeedOfTime( void ) const { return m_fSpeedOfTime; }
	void					SetSpeedOfTime( float fVal );
	bool					IsPaused( void ) const;
	void					SetPaused( bool bPaused );
	void					CreateDisturbance( CCircle& Disturbance );
	void					NotifySquirrelDeath( void );
	void					RestartGame( void );
	void					DrawGameOver( void );
	void					DrawDelayMessage( void );
	CSquirrel*				GetSquirrel( void ) { return m_pSquirrel; }
	bool					LoadLevel( UINT32 uLevelNum );
	void					UpdateAndRenderDisturbances( float fTimeDelta );


	void
	GetWalls(CWall*& _prWalls, int& _irNumWalls)
	{
		_prWalls = m_pWalls;
		_irNumWalls = m_uNumWalls;
	}


	void
	GetAcorns(CAcorn*& _prAcorns, int& _irNumAcorns)
	{
		_prAcorns = m_pAcorns;
		_irNumAcorns = m_uNumAcorns;
	}


	void
	GetMoles(CMole*& _prMoles, int& _irNumMoles)
	{
		_prMoles = m_pMoles;
		_irNumMoles = m_uNumMoles;
	}

	UINT32 GetNumLevels()
	{
		return (m_uNumLevels);
	}

	UINT32 GetCurrentLevelId()
	{
		return (m_uCurrentlLevel);
	}


#else // above is game specific, below is editor specific.
	bool Save( const char* strFileName );
	void SetEditTool( IEditorTool* pTool );
	bool CreateWalls( const CFVec2* pPositions, UINT32 uWallCount );
	bool CreateAcorn( const CFVec2& vPosition );
	bool CreateMole( const CFVec2& vPosition, float fHeading );
	bool CreateTunnel( const CFVec2& vPosition );
	void DeleteHighlighedItems( void );
#endif // end of editor specific code.
protected:
	CMMMContext& operator=(const CMMMContext& Other);	// non implemented assignment, fixes a warning.

	GDE::CRenderer*				m_pRenderer;			//!< The renderer used.
	GDE::CInputDevice*			m_pInputMgr;			//!< The input manager.
	GDE::CDataParserManager*	m_pParserManager;		//!< The data parser manager.
	GDE::CSpriteManager*		m_pSpriteManager;		//!< The sprite manager.

	GDE::CClock*				m_pFrameTimer;			//!< The frame timer, always at 'real' speed.
	GDE::CClock*				m_pGameTimer;			//!< The game timer, can be sped up, slowed down or paused.
	GDE::CFont*					m_pFont;				//!< The current font.
	CMouse*						m_pMouse;				//!< The mouse representation.
	CMap*						m_pMap;					//!< The map (used for A*)
	CWall*						m_pWalls;				//!< All the walls in the map.
	UINT32						m_uNumWalls;			//!< The number of walls.
	CAcorn*						m_pAcorns;				//!< All the acorns in the map.
	UINT32						m_uNumAcorns;			//!< The count of the acorns present.
	CMole*						m_pMoles;				//!< All the moles in the level.
	UINT32						m_uNumMoles;			//!< The number of moles in the level.
	CTunnel*					m_pTunnels;				//!< All the tunnels in the level.
	UINT32						m_uNumTunnels;			//!< How many tunnels are there?
#ifndef _EDITOR
	UINT32						m_uDisturbanceLogSize;	//!< How many disturbances can be stored.
	float						m_fDisturbanceLogMaxAge;//!< Max age (in seconds) to keep a disturbance for.
	struct SDisturbanceLog*		m_pDisturbanceLog;		//!< The disturbance log.
	UINT32						m_uNumLevels;			//!< The number of levels.
	UINT32						m_uCurrentlLevel;		//!< What is the current level?
	CSquirrel*					m_pSquirrel;			//!< The squirrel.
	float						m_fSpeedOfTime;			//!< The speed of time.
	float						m_fPlayerBaseDelay;		//!< How long does a player delay before starting a level.
	float						m_fPlayerDelay;			//!< How long left for the player to delay playing.
	int							m_iCurrentDifficulty;
	bool						m_bFirstRun;

	CAddLive					m_AddLive;
	CSkipLevel					m_SkipLevel;
	CSleepMoles					m_SleepMoles;

#else  // above is game specific, below is editor specific.
	IEditorTool*				m_pEditorTool;			//!< The current editor tool.
#endif	// end of editor specific code.
};

typedef CMMMContext CMMMContext; //!< Convenience, in editor it's the editor context, otherwise it's the game context.


















#endif //_MobMoleMadnessCONTEXT_H__Included_214923253

