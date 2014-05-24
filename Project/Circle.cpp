/**
 *	@file MobMoleMadness\Circle.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Circle.h"

#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"
#include "GDE/Renderer/Renderer.h"
#include "MMMContext.h"

#include <math.h>
/**
 *	@param vCentre	The centre of the circle to set.
 *	@param fRadius	The radius to set the circle to.
 */
void CCircle::Initialise( const CFVec2& vCentre, float fRadius )
{
	m_vCentre = vCentre;
	m_fRadius = fRadius;
}

/**
 *	@param Other	A circle to test against.
 *	@return			True if the two circles overlap.
 */
bool CCircle::Overlaps(const CCircle &Other) const
{
	const float fDistSq = m_vCentre.SquareDistance( Other.m_vCentre );
	const float fRadiusSum = m_fRadius + Other.m_fRadius;
	return fDistSq <= fRadiusSum*fRadiusSum;
}

/**
 *	@param vRayStart	Start point of an infinite ray.
 *	@param vRayOffset	Direction of the ray, parametric t multiplier.
 *	@param fT0			Filled in with time of first intersection (if present).
 *	@param fT1			Filled in with time of second intersection (if present).
 *	@return				The number of intersections found.
 */
UINT32 CCircle::GetLineIntersectionTimes(const CFVec2& vRayStart, const CFVec2& vRayOffset, float &fT0, float &fT1) const
{
	const CFVec2 vToRay = vRayStart - m_vCentre;
	// TODO: reformulate without the square root?
	const float fExtentsRecip = 1.0f/vRayOffset.Magnitude();
	const float fRayDotToRay = vToRay.DotProduct( vRayOffset ) * fExtentsRecip;
	const float fDiscr = fRayDotToRay*fRayDotToRay - (vToRay.SquareMagnitude()-m_fRadius*m_fRadius);
	if ( fDiscr <= 0.0f )
	{
		return 0;	// no intersections.
	}
	if ( fDiscr == 0.0f )
	{
		fT0 = -fRayDotToRay * fExtentsRecip;
		return 1;	// one intersection, just touching
	} else
	{
		const float fRoot = sqrtf( fDiscr );
		fT0 = (-fRayDotToRay - fRoot) * fExtentsRecip;
		fT1 = (-fRayDotToRay + fRoot) * fExtentsRecip;
		return 2;	// two intersections.
	}
}

/**
 *	@param v0	One end of line segment.
 *	@param v1	The other end of the line segment.
 *	@return Does the circle overlap the line segment?
 */
bool	CCircle::OverlapsLineSegment( const CFVec2& v0, const CFVec2& v1 ) const
{
	// calculate all intersections on the infinite line:
	float fIntersections[2];
	const UINT32 uNumIntersections = GetLineIntersectionTimes( v0, v1-v0, fIntersections[0], fIntersections[1] );
	for ( UINT32 i=0; i<uNumIntersections; i++ )
	{
		// is the intersection within the line segment?
		if ( fIntersections[i] >= 0.0f && fIntersections[i] <= 1.0f )
		{
			return true;
		}
	}
	return false;
}






void
CCircle::Draw(UINT32 _uiColour, bool _bFilled)
{
	GDE::CRenderer* pRenderer = CMMMContext::GetInstance().GetRenderer();
	pRenderer->GetTextureManager().GetNULLTexture()->Apply(0);


	if (_bFilled)
	{
		CMMMContext::GetInstance().DrawFilledCircle(*this, _uiColour);
	}
	else
	{
		CMMMContext::GetInstance().DrawCircle(*this, _uiColour);
	}
}