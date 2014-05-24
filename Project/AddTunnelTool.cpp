/**
 *	@file MobMoleMadness\AddTunnelTool.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "AddTunnelTool.h"
#include "Mouse.h"
#include "Circle.h"

#include "MMMContext.h"
#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"


class CAddTunnelLeftClickObserver : public GDE::IObserver
{
	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		UNUSED_VAR( pSubject );
		UNUSED_VAR( uMessage );
		CAddTunnelTool::GetInstance().NotifyLeftClick();
	}
};

//class CAddTunnelRightClickObserver : public GDE::IObserver
//{
//	// for the observer.
//	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
//	{
//		UNUSED_VAR( pSubject );
//		UNUSED_VAR( uMessage );
//		CAddTunnelTool::GetInstance().NotifyRightClick();
//	}
//};

static CAddTunnelLeftClickObserver	gs_AddTunnelLCObserver;
//static CAddWallRightClickObserver	gs_AddTunnelRCObserver;

CAddTunnelTool::CAddTunnelTool( void )
{
}

CAddTunnelTool& CAddTunnelTool::GetInstance( void )
{
	static CAddTunnelTool Instance;
	return Instance;
}

const char* CAddTunnelTool::GetDescription( void ) const
{
	if ( MouseIsValidPos() )
	{
		return "Add Tunnels: left click to place";
	} 
	return "Add Tunnels: Unable to place at current mouse position";
}

bool CAddTunnelTool::RegisterInput( GDE::CInputDevice* pDevice )
{
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_AddTunnelLCObserver, uCode );
	} else
	{
		assert_now( "Unable to bind input for left mouse" );
	}
	//if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_AddTunnelRCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to bind input for right mouse" );
	//}
	return true;
}

void CAddTunnelTool::ReleaseInput( GDE::CInputDevice* pDevice )
{
	CMMMContext::GetInstance().GetMouse()->SetValidPos( true ); // incase it was false.
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_AddTunnelLCObserver, uCode );
	} else
	{
		assert_now( "Unable to remove input for left mouse" );
	}
	//if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_AddTunnelRCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to remove input for right mouse" );
	//}
}

void CAddTunnelTool::NotifyLeftClick( void )
{
	if ( MouseIsValidPos() )
	{
		const CFVec2& vPos = CMMMContext::GetInstance().GetMouse()->GetPosition();
		CMMMContext::GetInstance().CreateTunnel( vPos );
	}
}

//void CAddTunnelTool::NotifyRightClick( void )
//{
//}

void CAddTunnelTool::Render( void )
{
	CMMMContext::GetInstance().GetMouse()->SetValidPos( MouseIsValidPos() );
}

bool CAddTunnelTool::MouseIsValidPos( void ) const
{
	const CFVec2& vPos = CMMMContext::GetInstance().GetMouse()->GetPosition();
	CCircle Bounds;
	Bounds.Initialise( vPos, fACORN_RADIUS );
	// now check to see if that intersects any walls or other such badness.
	return false == CMMMContext::GetInstance().CircleIntersectsAnything( Bounds );
}

