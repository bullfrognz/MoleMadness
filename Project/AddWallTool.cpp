/**
 *	@file MobMoleMadness\AddWallTool.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "AddWallTool.h"
#include "Mouse.h"

#include "MMMContext.h"
#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"

const float fHANDLE_DIST = 5.0f; //!< How far to snap onto vertices.

class CAddWallLeftClickObserver : public GDE::IObserver
{
	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		UNUSED_VAR( pSubject );
		UNUSED_VAR( uMessage );
		CAddWallTool::GetInstance().NotifyLeftClick();
	}
};

class CAddWallRightClickObserver : public GDE::IObserver
{
	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		UNUSED_VAR( pSubject );
		UNUSED_VAR( uMessage );
		CAddWallTool::GetInstance().NotifyRightClick();
	}
};

static CAddWallLeftClickObserver	gs_AddWallLCObserver;
static CAddWallRightClickObserver	gs_AddWallRCObserver;

CAddWallTool::CAddWallTool( void )
: m_uNumPoints( 0 )
{
}

CAddWallTool& CAddWallTool::GetInstance( void )
{
	static CAddWallTool Instance;
	return Instance;
}

const char* CAddWallTool::GetDescription( void ) const
{
	if ( m_uNumPoints == 0 )
	{
		return "Add Walls: left click to start placing";		
	} else
	{
		return "Add Walls: left click to place another vertex, reselect to close a group, right click to finish unclosed";
	}
}

bool CAddWallTool::RegisterInput( GDE::CInputDevice* pDevice )
{
	m_uNumPoints = 0;
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_AddWallLCObserver, uCode );
	} else
	{
		assert_now( "Unable to bind input for left mouse" );
	}
	if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	{
		pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_AddWallRCObserver, uCode );
	} else
	{
		assert_now( "Unable to bind input for right mouse" );
	}
	return true;
}

void CAddWallTool::ReleaseInput( GDE::CInputDevice* pDevice )
{
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_AddWallLCObserver, uCode );
	} else
	{
		assert_now( "Unable to remove input for left mouse" );
	}
	if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	{
		pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_AddWallRCObserver, uCode );
	} else
	{
		assert_now( "Unable to remove input for right mouse" );
	}
}

void CAddWallTool::NotifyLeftClick( void )
{
	CFVec2 vPos = CMMMContext::GetInstance().GetMouse()->GetPosition();
	if ( m_uNumPoints > 1 && vPos.SquareDistance( m_vSegmentEnds[0] ) < fHANDLE_DIST*fHANDLE_DIST )
	{
		// close the wall segment.
		m_vSegmentEnds[m_uNumPoints++] = m_vSegmentEnds[0];
		CMMMContext::GetInstance().CreateWalls( m_vSegmentEnds, m_uNumPoints/2 );
//#pragma warnmsg( "TODO: closed wall segments: fill?" )
		m_uNumPoints = 0;
		return;
	}
	// start the next segment:
	m_vSegmentEnds[m_uNumPoints++] = vPos;
	if ( m_uNumPoints > 1 )
	{
		// and finish the last one:
		m_vSegmentEnds[m_uNumPoints++] = vPos;
	}
}

void CAddWallTool::NotifyRightClick( void )
{
	if ( 0==m_uNumPoints )
	{
		DbgPrint( "Canceled an empty wall segment\n" );
		return;
	}
	m_uNumPoints--;	// delete the last segment.
	CMMMContext::GetInstance().CreateWalls( m_vSegmentEnds, m_uNumPoints/2 );
	m_uNumPoints = 0;
}

void CAddWallTool::Render( void )
{
	if ( 0 == m_uNumPoints )
	{
		return;
	}

	CFVec2 vPos = CMMMContext::GetInstance().GetMouse()->GetPosition();
	m_vSegmentEnds[m_uNumPoints] = vPos;
	if ( vPos.SquareDistance( m_vSegmentEnds[0] ) < fHANDLE_DIST*fHANDLE_DIST )
	{
		// close the wall segment, so show the snap.
		m_vSegmentEnds[m_uNumPoints] = m_vSegmentEnds[0];
	}

	for ( UINT32 i=0; i<m_uNumPoints; i+=2 )
	{
		// draw in red
		CMMMContext::GetInstance().DrawLine( m_vSegmentEnds[i], m_vSegmentEnds[i+1], 0xFFFF0000 );
	}	
}


