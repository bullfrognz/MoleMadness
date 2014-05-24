/**
 *	@file MobMoleMadness\LineSegment2D.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "LineSegment2D.h"
#include "Circle.h"

/**
 *	Ininitialise the line segment.
 *	@param v0	One end of the wall.
 *	@param v1	The other end of the wall.
 */
void CLineSegment2D::Initialise( const CFVec2& v0, const CFVec2& v1 )
{
	m_vEnd[0] = v0;
	m_vEnd[1] = v1;
}

/**
 *	@param Circle A circle to evaluate to see if it intersects the wall.
 *	@return true if the circle intersects the wall.
 */
bool CLineSegment2D::Intersects( const CCircle& Circle ) const
{
	return Circle.OverlapsLineSegment( m_vEnd[0], m_vEnd[1] );
}

/**
 *	Check this line segment with an infinite ray, given in parametric form and return the collision if applicable.
 *	@param vRayStart	Start point for parametric equation of infinite ray.
 *	@param vRayEnd		Direction vector (coefficient of t) for paremetric equation of infinitie ray.
 *	@param fT			To be filled in with the time (paremetric T) of intersection.
 *	@return				True if the intersection occurs within this line segment.
 */
bool CLineSegment2D::GetLineIntersectionTime(  const CFVec2& vRayStart, const CFVec2& vRayOffset, float& fT ) const
{
	// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
	const CFVec2& P1 = m_vEnd[0];
	const CFVec2& P2 = m_vEnd[1];
	const CFVec2& P3 = vRayStart;
	// no P4 required..
	const CFVec2 V1 = P2-P1;
	const CFVec2& V2 = vRayOffset;
	
	const float fDenom = V2.Y()*V1.X() - V2.X()*V1.Y();
	
	if ( fDenom == 0.0f )
	{
		return false; // no intersections, or the lines are the same!
	}

	const float fDRecip = 1.0f/fDenom;
	// and then, finally Q (for x/y1 - x/y3)
	const CFVec2 Q = P1-P3;

	const float fTimeThis = (V2.X()*Q.Y() - V2.Y()*Q.X())*fDRecip;

	if ( fTimeThis < 0.0f || fTimeThis > 1.0f )
	{
		// outside of the line segment.
		return false;
	}
	fT = (V1.X()*Q.Y() - V1.Y()*Q.X())*fDRecip;
	return true;
}

/**
 *	Check this line segment, extending it to an infinite ray with an infinite ray, given in parametric form and return the collision if applicable.
 *	@param vRayStart	Start point for parametric equation of infinite ray.
 *	@param vRayEnd		Direction vector (coefficient of t) for paremetric equation of infinitie ray.
 *	@param fTOther		To be filled in with the time (paremetric T) of intersection, with respect to the other line.
 *	@param fTThis		To be filled in with the time (paremetric T) of intersection, with respect to this line, with dir being End1-End0.
 *	@return				True if the intersection occurs within this line segment.
 */
bool CLineSegment2D::GetLineIntersectionTimes( const CFVec2& vRayStart, const CFVec2& vRayOffset, float& fTOther, float& fTThis ) const
{
	// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
	const CFVec2& P1 = m_vEnd[0];
	const CFVec2& P2 = m_vEnd[1];
	const CFVec2& P3 = vRayStart;
	// no P4 required..
	const CFVec2 V1 = P2-P1;
	const CFVec2& V2 = vRayOffset;
	
	const float fDenom = V2.Y()*V1.X() - V2.X()*V1.Y();
	
	if ( fDenom == 0.0f )
	{
		return false; // no intersections, or the lines are the same!
	}

	const float fDRecip = 1.0f/fDenom;
	// and then, finally Q (for x/y1 - x/y3)
	const CFVec2 Q = P1-P3;

	fTThis = (V2.X()*Q.Y() - V2.Y()*Q.X())*fDRecip;
	fTOther = (V1.X()*Q.Y() - V1.Y()*Q.X())*fDRecip;
	return true;	
}

/**
 *	Get the closest point on the line segment to the given point.
 *	@vPoint		The point to find the closest too.
 *	@vClosest	Returned as the closest point.
 *	@return		Success, should never fail.
 */
bool CLineSegment2D::GetClosestPointToPoint( const CFVec2& vPoint, CFVec2& vClosest ) const
{
	CFVec2 vRayDir = m_vEnd[1]-m_vEnd[2];
	vRayDir.SetPerpendicular();
	float fTOther, fTThis;
	if ( GetLineIntersectionTimes( vPoint, vRayDir, fTOther, fTThis ) )
	{
		if ( fTThis <= 0.0f )
		{
			vClosest = m_vEnd[0];
			return true;
		}
		if ( fTThis >= 1.0f )
		{
			vClosest = m_vEnd[1];
			return true;
		}

		vClosest = vPoint + vRayDir * fTOther;
		return true;
	}
	assert_now( "Should be unreachable" );
	return false;
}

bool CLineSegment2D::Intersects( const CLineSegment2D& Other ) const
{
	float fTime;
	if ( GetLineIntersectionTime( Other.m_vEnd[0], Other.m_vEnd[1]-Other.m_vEnd[0], fTime ) )
	{
		return fTime >= 0.0f && fTime <= 1.0f;
	}
	return false;
}
