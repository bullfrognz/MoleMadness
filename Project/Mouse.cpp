/**
 *	@file MobMoleMadness\Mouse.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Mouse.h"
#include "MMMContext.h"
#include "Circle.h"

#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"

CMouse::CMouse( void )
: m_vPosition( SFVec2( 0.0f, 0.0f ) )
, m_fVisualRadius( 8.0f )
, m_bValidPos( true )
{	// empty
}
/**
 *	Update the mouse position based on the input device.
 *	Requires to be listening to a mouseXY state.
 */
void CMouse::NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
{
	const GDE::CInputDevice& Device = reinterpret_cast<GDE::IInputChangeSubject*>(pSubject)->GetDevice();	
	const GDE::SInputState& State = Device.GetState( uMessage );
	m_vPosition.X( State.m_fValue );
	m_vPosition.Y( State.m_fValue2 );

}

/**
 *	Draw the mouse
 *	TODO: better drawing!
 */
void CMouse::Draw( void )
{
	CCircle Rep;
	GetBoundingCircle( Rep );
	if ( m_bValidPos )
	{
		CMMMContext::GetInstance().DrawCircle( Rep, 0xFF003F00 );
	} else
	{
		CMMMContext::GetInstance().DrawCircle( Rep, 0xFFFF0000 );
	}
}

void CMouse::GetBoundingCircle( CCircle& Bounds )
{
	Bounds.Initialise( m_vPosition, m_fVisualRadius );
}
