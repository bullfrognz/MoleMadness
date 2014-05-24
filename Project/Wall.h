/**
 *	@file MobMoleMadness\Wall.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _WALL_H__Included_1049460091
#define _WALL_H__Included_1049460091

#include "LineSegment2D.h"
#include "Repulsor.h"

class CBox2D;

/**
 *	This class represents a wall in the world representation.
 */
class CWall
{
public:
	// no need for constructor, initialised from a list.
	void Initialise( const CFVec2& v0, const CFVec2& v1 );
	void Draw( void );
	bool Intersects( const CCircle& Circle ) const;
	bool Intersects( const CBox2D& Box ) const;

	bool GetLineIntersectionTime( const CFVec2& vRayStart, const CFVec2& vRayOffset, float& fT ) const;

	void SetHighlighted( bool bHighlighted ) { m_bHighlighted = bHighlighted; }
	bool IsHighlighted( void ) const { return m_bHighlighted; }

	const CFVec2&	GetEnd0( void ) const { return m_Line.GetEnd0(); }
	const CFVec2&	GetEnd1( void ) const { return m_Line.GetEnd1(); }

	const CFVec2& GetStartPoint() const { return m_Line.GetEnd0(); }
	const CFVec2& GetEndPoint() const { return m_Line.GetEnd1(); }

	CFVec2 GetNormal() const;


	inline int GetNumRepulsors() { return (2); }
	inline CRepulsor& GetRepulsor(int _iIndex) { return (m_Repulsor[_iIndex]); }

protected:
	CLineSegment2D	m_Line;
	bool			m_bHighlighted;		//!< Is this wall currently highlighted;


	CRepulsor m_Repulsor[2];
};

#endif //_WALL_H__Included_1049460091

