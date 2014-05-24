/**
 *	@file MobMoleMadness\SelectBoxTool.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "SelectBoxTool.h"
#include "Mouse.h"
#include "Circle.h"
#include "Box2D.h"

#include "MMMContext.h"
#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"

class CSelectBoxLeftClickObserver : public GDE::IObserver
{
public:
	CSelectBoxLeftClickObserver( ISelectBoxTool& Tool )
		: m_Tool( Tool )
	{	// empty
	}
	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		GDE::CInputDevice& Device = reinterpret_cast<GDE::IInputChangeSubject*>(pSubject)->GetDevice();
		if ( Device.GetState( uMessage ).m_bButtonDown )
		{
			m_Tool.NotifyLeftClickDown();
		} else
		{
			m_Tool.NotifyLeftClickUp();
		}
	}
protected:
	void operator =( const CSelectBoxLeftClickObserver& Other );
	ISelectBoxTool& m_Tool;
};

ISelectBoxTool::ISelectBoxTool( void )
: m_bSelectionActive( false )
{
	m_pObserver = new CSelectBoxLeftClickObserver(*this);
}

ISelectBoxTool::~ISelectBoxTool( void )
{
	delete m_pObserver;
}

bool ISelectBoxTool::RegisterInput( GDE::CInputDevice* pDevice )
{
	CMMMContext::GetInstance().ClearAllHighlights();
	m_bSelectionActive = false;
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetChangeSubject().AddObserver( m_pObserver, uCode );
	} else
	{
		assert_now( "Unable to bind input for left mouse" );
	}
	return true;
}

void ISelectBoxTool::ReleaseInput( GDE::CInputDevice* pDevice )
{
	CMMMContext::GetInstance().ClearAllHighlights();
	CMMMContext::GetInstance().GetMouse()->SetValidPos( true ); // incase it was false.
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetChangeSubject().RemoveObserver( m_pObserver, uCode );
	} else
	{
		assert_now( "Unable to remove input for left mouse" );
	}
}

void ISelectBoxTool::NotifyLeftClickDown( void )
{
	m_bSelectionActive = true;
	m_vFirstCorner = CMMMContext::GetInstance().GetMouse()->GetPosition();
}

void ISelectBoxTool::NotifyLeftClickUp( void )
{
	if ( m_bSelectionActive )
	{
		m_bSelectionActive = false;
		if( SelectionComplete( CMMMContext::GetInstance().GetMouse()->GetPosition() ) )
		{
			CMMMContext::GetInstance().SetEditTool( IEditorTool::GetTool( ETool_SelectTool ) );
		}
	}
}

void ISelectBoxTool::Render( void )
{
	if ( m_bSelectionActive )
	{
		const CFVec2& vPos = CMMMContext::GetInstance().GetMouse()->GetPosition();
		CBox2D Box;
		Box.InitCorners( m_vFirstCorner, vPos );
		CMMMContext::GetInstance().DrawBox( Box, 0xFF00FF00 );

//#pragma warnmsg( "This would be better in an update function" )
		CMMMContext::GetInstance().ClearAllHighlights();
		CMMMContext::GetInstance().HighlightBoxIntersections( Box );
	}
}
