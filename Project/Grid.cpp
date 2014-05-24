/**
 *	@file MobMoleMadness\Grid.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Grid.h"
#include "Tile.h"
#include "Map.h"
#include "MMMContext.h"

#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/Vertex.h"
// for untextured:
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"

const static SGridRef gs_NeighbourOffsets[] = 
{
	{  0, -1 }, //GD_North = 0,
	{  1, -1 }, //GD_NorthEast,
	{  1,  0 }, //GD_East,
	{  1,  1 }, //GD_SouthEast,
	{  0,  1 }, //GD_South,
	{ -1,  1 }, //GD_SouthWest,
	{ -1,  0 }, //GD_West,
	{ -1, -1 }, //GD_NorthWest
};

bool CGrid::Init( CTile* pTile )
{
	m_pTile = pTile;

	// create the corners:
	SGridRef Pos;
	CMMMContext::GetInstance().GetMap()->GetGridRef( this, Pos );
	m_fv2Centre = SFVec2( 
		float(Pos.m_iX * uTILE_WIDTH + uTILE_WIDTH/2),
		float(Pos.m_iY * uTILE_HEIGHT + uTILE_HEIGHT/2 ) );
	m_fv2Corners[0] = m_fv2Centre;
	m_fv2Corners[1] = m_fv2Centre;
	m_fv2Corners[2] = m_fv2Centre;
	m_fv2Corners[3] = m_fv2Centre;
	const float fHalfTile = float( uTILE_WIDTH/2 );
	m_fv2Corners[0].XAdd( -fHalfTile );
	m_fv2Corners[0].YAdd( -fHalfTile );
	m_fv2Corners[1].XAdd( fHalfTile );
	m_fv2Corners[1].YAdd( -fHalfTile );
	m_fv2Corners[2].XAdd( fHalfTile );
	m_fv2Corners[2].YAdd( fHalfTile );
	m_fv2Corners[3].XAdd( -fHalfTile );
	m_fv2Corners[3].YAdd( fHalfTile );
	return true;
}

void CGrid::Draw( void )
{
	SGridRef Pos;
	CMMMContext::GetInstance().GetMap()->GetGridRef( this, Pos );
	m_pTile->Draw( (UINT32)Pos.m_iX, (UINT32)Pos.m_iY );
}

void CGrid::DrawCross( UINT32 colCross ) const
{
	GDE::SVertex Verts[4];
	Verts[0].m_fX = m_fv2Corners[0].X();
	Verts[0].m_fY = m_fv2Corners[0].Y();

	Verts[1].m_fX = m_fv2Corners[2].X();
	Verts[1].m_fY = m_fv2Corners[2].Y();

	Verts[2].m_fX = m_fv2Corners[1].X();
	Verts[2].m_fY = m_fv2Corners[1].Y();

	Verts[3].m_fX = m_fv2Corners[3].X();
	Verts[3].m_fY = m_fv2Corners[3].Y();

	for ( UINT32 i=0; i<4; i++ )
	{
		Verts[i].m_fZ = 0.5f;
		Verts[i].m_uColour = colCross;
	}
	
	// untextured:
	CMMMContext::GetInstance().GetRenderer()->GetTextureManager().GetNULLTexture()->Apply( 0 );
	CMMMContext::GetInstance().GetRenderer()->DrawPrimitives( GDE::PT_LineList, 2, Verts );

}

void		CGrid::DrawOutline( UINT32 colOutline ) const
{
	GDE::SVertex Verts[8];
	Verts[0].m_fX = m_fv2Corners[0].X();
	Verts[0].m_fY = m_fv2Corners[0].Y();

	Verts[1].m_fX = m_fv2Corners[1].X();
	Verts[1].m_fY = m_fv2Corners[1].Y();

	Verts[2].m_fX = m_fv2Corners[1].X();
	Verts[2].m_fY = m_fv2Corners[1].Y();

	Verts[3].m_fX = m_fv2Corners[2].X();
	Verts[3].m_fY = m_fv2Corners[2].Y();

	Verts[4].m_fX = m_fv2Corners[2].X();
	Verts[4].m_fY = m_fv2Corners[2].Y();

	Verts[5].m_fX = m_fv2Corners[3].X();
	Verts[5].m_fY = m_fv2Corners[3].Y();

	Verts[6].m_fX = m_fv2Corners[3].X();
	Verts[6].m_fY = m_fv2Corners[3].Y();

	Verts[7].m_fX = m_fv2Corners[0].X();
	Verts[7].m_fY = m_fv2Corners[0].Y();

	for ( UINT32 i=0; i<8; i++ )
	{
		Verts[i].m_fZ = 0.5f;
		Verts[i].m_uColour = colOutline;
	}
	
	// untextured:
	CMMMContext::GetInstance().GetRenderer()->GetTextureManager().GetNULLTexture()->Apply( 0 );
	CMMMContext::GetInstance().GetRenderer()->DrawPrimitives( GDE::PT_LineList, 4, Verts );	 
}

void CGrid::DrawArrowTo( const CGrid* pTarget, UINT32 colArrow, float fArrowDist ) const
{
	// Get the offset between the two grid centres:
	const CFVec2 vOffet = (pTarget->m_fv2Centre-m_fv2Centre).GetNormal();
	const float fCentreDist = 5.0f;
	const CFVec2 vStart = m_fv2Centre+vOffet*fCentreDist;
	const CFVec2 vEnd = pTarget->m_fv2Centre-vOffet*fCentreDist;
	const CFVec2 vArrowHead = vStart + (vEnd-vStart)*fArrowDist;
	CFVec2 vPerp = vOffet;
	vPerp.SetPerpendicular();
	const float fArrowSize = 5.0f;
	const CFVec2 vArrowEnd1 = vArrowHead + vPerp*fArrowSize - vOffet*fArrowSize;
	const CFVec2 vArrowEnd2 = vArrowHead - vPerp*fArrowSize - vOffet*fArrowSize;

	// great, now the positions are known:
	GDE::SVertex Verts[6];
	Verts[0].m_fX = vStart.X();
	Verts[0].m_fY = vStart.Y();
	Verts[1].m_fX = vEnd.X();
	Verts[1].m_fY = vEnd.Y();
	Verts[2].m_fX = vArrowHead.X();
	Verts[2].m_fY = vArrowHead.Y();
	Verts[3].m_fX = vArrowEnd1.X();
	Verts[3].m_fY = vArrowEnd1.Y();
	Verts[4].m_fX = vArrowHead.X();
	Verts[4].m_fY = vArrowHead.Y();
	Verts[5].m_fX = vArrowEnd2.X();
	Verts[5].m_fY = vArrowEnd2.Y();

	for ( UINT32 i=0; i<6; i++ )
	{
		Verts[i].m_fZ = 0.5f;
		Verts[i].m_uColour = colArrow;
	}
	
	// untextured:
	CMMMContext::GetInstance().GetRenderer()->GetTextureManager().GetNULLTexture()->Apply( 0 );
	CMMMContext::GetInstance().GetRenderer()->DrawPrimitives( GDE::PT_LineList, 3, Verts );
}

SINT32		CGrid::GetCost( void ) const
{
	return m_pTile->GetCost();
}

const char*	CGrid::GetName( void ) const
{
	return m_pTile->GetName();
}

/**
 *	@return the neighbour grid in the offset provided, or NULL if none.
 */
CGrid*	CGrid::GetNeighbour( EGridDirection eDir )
{
	SGridRef Pos;
	CMMMContext::GetInstance().GetMap()->GetGridRef( this, Pos );
	Pos.m_iX += gs_NeighbourOffsets[eDir].m_iX;
	Pos.m_iY += gs_NeighbourOffsets[eDir].m_iY;
	return CMMMContext::GetInstance().GetMap()->GetGrid( 
		UINT32(Pos.m_iX),
		UINT32(Pos.m_iY) );
}

void	CGrid::InitNeighbours( void )
{
	m_uNumMoveableNeighbours = 0;
	for ( UINT32 i=0; i<8; i++ )
	{
		CGrid* pNext = GetNeighbour( EGridDirection(i) );
		if ( pNext && pNext->GetCost() >= 0 )
		{
			m_pMoveableNeighbours[m_uNumMoveableNeighbours++] = pNext;
		}
	}
}


/**
 *	@return The number of neighbours that can be moved to from here.
 */
UINT32	CGrid::GetMovableNeighbourCount( void ) const
{
	return m_uNumMoveableNeighbours;
}

/**
 *	@param The index to get, goes from zero the number of movable neighbours. @see GetMovableNeighbourCount
 *	@return The movable neighbour at the given index.
 */
CGrid*	CGrid::GetMovableNeighbour( UINT32 uIndex )
{
	if ( uIndex < m_uNumMoveableNeighbours )
	{
		return m_pMoveableNeighbours[uIndex];
	} 
	assert_now( "Invalid neighbour index %d, this grid has %d", uIndex, m_uNumMoveableNeighbours );
	return NULL;
}
