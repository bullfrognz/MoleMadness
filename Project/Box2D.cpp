/**
 *	@file MobMoleMadness\Box2D.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Box2D.h"
#include "Circle.h"
#include "LineSegment2D.h"
#include "MMMContext.h"


#include "GDE/Renderer/Renderer.h"

#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"

/**
 *	@param vCentre	Centre of the new box.
 *	@param vExtents	Extents of the new box.
 */
bool	CBox2D::InitExtents( const CFVec2& vCentre, const CFVec2& vExtents )
{
	m_vCentre = vCentre;
	m_vExtents = vExtents;
	ComputeVertices();

	return true;
}

/**
 *	@param v0	One corner of the new box.
 *	@param v1	The diaganol opposite corner of the new box.
 */
bool	CBox2D::InitCorners( const CFVec2& v0, const CFVec2& v1 )
{
	m_vCentre = (v0+v1)*0.5f;
	m_vExtents = v1-m_vCentre;
	m_vExtents.SetAbs();
	ComputeVertices();
	return true;
}

bool	CBox2D::Intersects( const CCircle& Circle ) const
{
	CFVec2 vOffset = (Circle.Centre() - m_vCentre);
	vOffset.SetAbs();
	vOffset -= m_vExtents;
	return ( vOffset.X() < Circle.Radius() && vOffset.Y() < Circle.Radius() );
}

bool	CBox2D::Intersects( const CBox2D& Box ) const
{
	CFVec2 vOffset = (Box.m_vCentre - m_vCentre);
	vOffset.SetAbs();
	vOffset -= m_vExtents;
	vOffset -= Box.m_vExtents;
	return ( vOffset.X() < 0.0f && vOffset.Y() < 0.0f );

}

bool	CBox2D::OverlapsLineSegment( const CFVec2& v0, const CFVec2& v1 ) const
{
	if ( ContainsPoint( v0 ) || ContainsPoint( v1 ) )
	{
		return true;
	}

	CLineSegment2D Seg;
	Seg.Initialise( v0, v1 );
	CLineSegment2D Edge;
	CFVec2 vCorners[5];
	vCorners[0] = m_vCentre-m_vExtents;
	vCorners[4] = vCorners[0];
	vCorners[1].X( m_vCentre.X() + m_vExtents.X() );
	vCorners[1].Y( m_vCentre.Y() - m_vExtents.Y() );
	vCorners[2] = m_vCentre+m_vExtents;
	vCorners[3].X( m_vCentre.X() - m_vExtents.X() );
	vCorners[3].Y( m_vCentre.Y() + m_vExtents.Y() );
	for ( UINT32 i = 0; i<4; i++ )
	{
		Edge.Initialise( vCorners[i], vCorners[i+1] );
		if ( Edge.Intersects( Seg ) )
		{
			return true;
		}
	}
	return false;
}

bool	CBox2D::ContainsPoint( const CFVec2& vPoint ) const
{
	CFVec2 vOffset = (vPoint - m_vCentre);
	vOffset.SetAbs();
	vOffset -= m_vExtents;
	return ( vOffset.X() < 0.0f && vOffset.Y() < 0.0f );

}





void
CBox2D::Draw(int _iColour)
{
	for ( UINT32 i=0; i<8; i++ )
	{
		m_Verts[i].m_uColour = _iColour;
	}


	CMMMContext::GetInstance().GetRenderer()->DrawPrimitives( GDE::PT_LineList, 4, m_Verts );	 
}



void
CBox2D::ComputeVertices()
{
	// Top
	m_Verts[0].m_fX = m_vCentre.X() - m_vExtents.X();
	m_Verts[0].m_fY = m_vCentre.Y() - m_vExtents.Y();
	m_Verts[1].m_fX = m_vCentre.X() + m_vExtents.X();
	m_Verts[1].m_fY = m_vCentre.Y() - m_vExtents.Y();


	// Right
	m_Verts[2].m_fX = m_vCentre.X() + m_vExtents.X();
	m_Verts[2].m_fY = m_vCentre.Y() - m_vExtents.Y();
	m_Verts[3].m_fX = m_vCentre.X() + m_vExtents.X();
	m_Verts[3].m_fY = m_vCentre.Y() + m_vExtents.Y();

	// Bottom
	m_Verts[4].m_fX = m_vCentre.X() + m_vExtents.X();
	m_Verts[4].m_fY = m_vCentre.Y() + m_vExtents.Y();
	m_Verts[5].m_fX = m_vCentre.X() - m_vExtents.X();
	m_Verts[5].m_fY = m_vCentre.Y() + m_vExtents.Y();


	// Left
	m_Verts[6].m_fX = m_vCentre.X() - m_vExtents.X();
	m_Verts[6].m_fY = m_vCentre.Y() - m_vExtents.Y();
	m_Verts[7].m_fX = m_vCentre.X() - m_vExtents.X();
	m_Verts[7].m_fY = m_vCentre.Y() + m_vExtents.Y();


	for ( UINT32 i=0; i<8; i++ )
	{
		m_Verts[i].m_fZ = 0.0f;
	}
}