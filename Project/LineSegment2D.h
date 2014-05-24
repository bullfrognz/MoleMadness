/**
 *	@file MobMoleMadness\LineSegment2D.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _LINESEGMENT2D_H__Included_2089235880
#define _LINESEGMENT2D_H__Included_2089235880

#include "FVector2.h"

class CCircle;

/**
 *	A 2D line segment.
 */
class CLineSegment2D
{
public:
	void Initialise( const CFVec2& v0, const CFVec2& v1 );

	bool Intersects( const CCircle& Circle ) const;
	bool Intersects( const CLineSegment2D& Other ) const;
	bool GetLineIntersectionTime( const CFVec2& vRayStart, const CFVec2& vRayOffset, float& fT ) const;
	bool GetLineIntersectionTimes( const CFVec2& vRayStart, const CFVec2& vRayOffset, float& fTOther, float& fTThis ) const;
	bool GetClosestPointToPoint( const CFVec2& vPoint, CFVec2& vClosest ) const;

	const CFVec2&	GetEnd0( void ) const { return m_vEnd[0]; }
	const CFVec2&	GetEnd1( void ) const { return m_vEnd[1]; }
protected:
	CFVec2	m_vEnd[2];			//!< The end points of the wall's line.
};

#endif //_LINESEGMENT2D_H__Included_2089235880

