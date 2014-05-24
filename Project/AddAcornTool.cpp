/**
 *	@file MobMoleMadness\AddAcornTool.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "AddAcornTool.h"
#include "Mouse.h"
#include "Circle.h"

#include "MMMContext.h"
#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"


class CAddAcornLeftClickObserver : public GDE::IObserver
{
	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		UNUSED_VAR( pSubject );
		UNUSED_VAR( uMessage );
		CAddAcornTool::GetInstance().NotifyLeftClick();
	}
};

//class CAddAcornRightClickObserver : public GDE::IObserver
//{
//	// for the observer.
//	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
//	{
//		UNUSED_VAR( pSubject );
//		UNUSED_VAR( uMessage );
//		CAddAcornTool::GetInstance().NotifyRightClick();
//	}
//};

static CAddAcornLeftClickObserver	gs_AddAcornLCObserver;
//static CAddWallRightClickObserver	gs_AddAcornRCObserver;

CAddAcornTool::CAddAcornTool( void )
{
}

CAddAcornTool& CAddAcornTool::GetInstance( void )
{
	static CAddAcornTool Instance;
	return Instance;
}

const char* CAddAcornTool::GetDescription( void ) const
{
	if ( MouseIsValidPos() )
	{
		return "Add Acorns: left click to place";
	} 
	return "Add Acorns: Unable to place at current mouse position";
}

bool CAddAcornTool::RegisterInput( GDE::CInputDevice* pDevice )
{
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_AddAcornLCObserver, uCode );
	} else
	{
		assert_now( "Unable to bind input for left mouse" );
	}
	//if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_AddAcornRCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to bind input for right mouse" );
	//}
	return true;
}

void CAddAcornTool::ReleaseInput( GDE::CInputDevice* pDevice )
{
	CMMMContext::GetInstance().GetMouse()->SetValidPos( true ); // incase it was false.
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_AddAcornLCObserver, uCode );
	} else
	{
		assert_now( "Unable to remove input for left mouse" );
	}
	//if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_AddAcornRCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to remove input for right mouse" );
	//}
}

void CAddAcornTool::NotifyLeftClick( void )
{
	if ( MouseIsValidPos() )
	{
		const CFVec2& vPos = CMMMContext::GetInstance().GetMouse()->GetPosition();
		CMMMContext::GetInstance().CreateAcorn( vPos );
	}
}

//void CAddAcornTool::NotifyRightClick( void )
//{
//}

void CAddAcornTool::Render( void )
{
	CMMMContext::GetInstance().GetMouse()->SetValidPos( MouseIsValidPos() );
}

bool CAddAcornTool::MouseIsValidPos( void ) const
{
	const CFVec2& vPos = CMMMContext::GetInstance().GetMouse()->GetPosition();
	CCircle Bounds;
	Bounds.Initialise( vPos, fACORN_RADIUS );
	// now check to see if that intersects any walls or other such badness.
	return false == CMMMContext::GetInstance().CircleIntersectsAnything( Bounds );
}

