/**
 *	@file MobMoleMadness\Circle.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _CIRCLE_H__Included_1340580524
#define _CIRCLE_H__Included_1340580524

#include "Common.h"
#include "FVector2.h"

/**
 *	A two dimensional circle.
 */
class CCircle
{
public:
	// no explicit constructor, instead initialise.
	void		Initialise( const CFVec2& vCentre, float fRadius );
	bool		Overlaps( const CCircle& Other ) const;
	bool		OverlapsLineSegment( const CFVec2& v0, const CFVec2& v1 ) const;
	UINT32		GetLineIntersectionTimes( const CFVec2& vRayStart, const CFVec2& vRayOffset, float& fT0, float& fT1 ) const;
	float		Radius( void ) const { return m_fRadius; }
	const CFVec2&	Centre( void ) const { return m_vCentre; }

	void			Draw(unsigned int _uiColour, bool _bFilled = false);

protected:
	CFVec2	m_vCentre;	//!< The circle's centre.
	float	m_fRadius;	//!< The circle's radius.
};

#endif //_CIRCLE_H__Included_1340580524

