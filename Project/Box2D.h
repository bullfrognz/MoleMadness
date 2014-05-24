/**
 *	@file MobMoleMadness\Box2D.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _BOX2D_H__Included_129105959
#define _BOX2D_H__Included_129105959

#include "FVector2.h"
#include "GDE/Renderer/Vertex.h"

class CCircle;

class CBox2D
{
public:
	bool		InitExtents( const CFVec2& vCentre, const CFVec2& vExtents );
	bool		InitCorners( const CFVec2& v0, const CFVec2& v1 );


	void		Draw(int _iColour);


	bool		Intersects( const CCircle& Circle ) const;
	bool		Intersects( const CBox2D& Box ) const;
	bool		OverlapsLineSegment( const CFVec2& v0, const CFVec2& v1 ) const;
	bool		ContainsPoint( const CFVec2& vPoint ) const;
	const CFVec2&	GetCentre( void ) const { return m_vCentre; }
	const CFVec2&	GetExtents( void ) const { return m_vExtents; }
protected:

	void ComputeVertices();

	CFVec2 m_vCentre;		//!< The centre point of the box.
	CFVec2 m_vExtents;		//!< The box Extents.


	GDE::SVertex m_Verts[8];
};

#endif //_BOX2D_H__Included_129105959

