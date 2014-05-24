/**
 *	@file MobMoleMadness\AddMoleTool.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "AddMoleTool.h"
#include "Mouse.h"
#include "Circle.h"

#include "MMMContext.h"
#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"

#include <Math.h> // atan2f

class CAddMoleLeftClickObserver : public GDE::IObserver
{
	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		UNUSED_VAR( pSubject );
		UNUSED_VAR( uMessage );
		CAddMoleTool::GetInstance().NotifyLeftClick();
	}
};

//class CAddMoleRightClickObserver : public GDE::IObserver
//{
//	// for the observer.
//	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
//	{
//		UNUSED_VAR( pSubject );
//		UNUSED_VAR( uMessage );
//		CAddMoleTool::GetInstance().NotifyRightClick();
//	}
//};

static CAddMoleLeftClickObserver	gs_AddMoleLCObserver;
//static CAddWallRightClickObserver	gs_AddMoleRCObserver;

CAddMoleTool::CAddMoleTool( void )
{
}

CAddMoleTool& CAddMoleTool::GetInstance( void )
{
	static CAddMoleTool Instance;
	return Instance;
}

const char* CAddMoleTool::GetDescription( void ) const
{
	if ( m_bGetRotation )
	{
		return "Add Moles: move mouse to give offset for heading and click";
	}

	if ( MouseIsValidPos() )
	{
		return "Add Moles: left click to place";
	} 

	return "Add Moles: Unable to place at current mouse position";
}

bool CAddMoleTool::RegisterInput( GDE::CInputDevice* pDevice )
{
	m_bGetRotation = false;
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_AddMoleLCObserver, uCode );
	} else
	{
		assert_now( "Unable to bind input for left mouse" );
	}
	//if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_AddMoleRCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to bind input for right mouse" );
	//}
	return true;
}

void CAddMoleTool::ReleaseInput( GDE::CInputDevice* pDevice )
{
	CMMMContext::GetInstance().GetMouse()->SetValidPos( true ); // incase it was false.
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_AddMoleLCObserver, uCode );
	} else
	{
		assert_now( "Unable to remove input for left mouse" );
	}
	//if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_AddMoleRCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to remove input for right mouse" );
	//}
}

void CAddMoleTool::NotifyLeftClick( void )
{
	if ( m_bGetRotation )
	{
		const CFVec2& vPos = CMMMContext::GetInstance().GetMouse()->GetPosition();
		if ( vPos == m_vMolePos )
		{
			CMMMContext::GetInstance().CreateMole( m_vMolePos, 0.0f );
		} else
		{
			const CFVec2 vOffset = vPos - m_vMolePos;
			const float fHeading = atan2f(  vOffset.X(), -vOffset.Y() );
			CMMMContext::GetInstance().CreateMole( m_vMolePos, fHeading );
		}
		m_bGetRotation = false;
		return;
	}
	if ( MouseIsValidPos() )
	{
		m_vMolePos = CMMMContext::GetInstance().GetMouse()->GetPosition();
		m_bGetRotation = true;
	}
}

//void CAddMoleTool::NotifyRightClick( void )
//{
//}

void CAddMoleTool::Render( void )
{
	if ( m_bGetRotation )
	{
		CMMMContext::GetInstance().GetMouse()->SetValidPos( true );
		const CFVec2& vPos = CMMMContext::GetInstance().GetMouse()->GetPosition();
		CMMMContext::GetInstance().DrawLine( vPos, m_vMolePos, 0xFFFF0000 );
		return;
	}
	CMMMContext::GetInstance().GetMouse()->SetValidPos( MouseIsValidPos() );

}

bool CAddMoleTool::MouseIsValidPos( void ) const
{
	const CFVec2& vPos = CMMMContext::GetInstance().GetMouse()->GetPosition();
	CCircle Bounds;
	Bounds.Initialise( vPos, fMOLE_RADIUS );
	// now check to see if that intersects any walls or other such badness.
	return false == CMMMContext::GetInstance().CircleIntersectsAnything( Bounds );
}

