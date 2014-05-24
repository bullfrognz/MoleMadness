/**
 *	@file MobMoleMadness\Mouse.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _MOUSE_H__Included_1578817441
#define _MOUSE_H__Included_1578817441

#include "FVector2.h"
#include "GDE/Core/Observer.h"

class CCircle;

class CMouse : public GDE::IObserver
{
public:
	CMouse( void );
	void Draw( void );
	void GetBoundingCircle( CCircle& Bounds );

	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage );
	const CFVec2&	GetPosition( void ) const { return m_vPosition; }
	void		SetValidPos( bool bValid ) { m_bValidPos = bValid; }
protected:
	CFVec2	m_vPosition;
	float	m_fVisualRadius;
	bool	m_bValidPos;					//!< Should the mouse show a valid position?
	void operator = (const CMouse& Other);	// remove warning, not implemented.
};

#endif //_MOUSE_H__Included_1578817441
