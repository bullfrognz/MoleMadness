/**
 *	@file MobMoleMadness\Grid.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _GRID_H__Included_1432030984
#define _GRID_H__Included_1432030984

#include "FVector2.h"

namespace GDE
{
	struct SVertex;
};

class CTile;

struct SGridRef
{
	SINT32 m_iX;
	SINT32 m_iY;
};

/**
 *	A handy grid direction, runs from zero to eight
 *	that can be used to get neighbour grids...
 */
enum EGridDirection
{
	GD_North = 0,
	GD_NorthEast,
	GD_East,
	GD_SouthEast,
	GD_South,
	GD_SouthWest,
	GD_West,
	GD_NorthWest
};

class CGrid
{
public:
	bool		Init( CTile* pTile );
	void		InitNeighbours( void );
	void		Draw( void );
	void		DrawCross( UINT32 colCross ) const;
	void		DrawOutline( UINT32 colOutline ) const;
	void		DrawArrowTo( const CGrid* pTarget, UINT32 colArrow, float fArrowDist ) const;
	SGridRef	GetRef( void );
	SINT32		GetCost( void ) const;
	const char*	GetName( void ) const;
	const CFVec2&	GetCentre( void ) const { return m_fv2Centre; }
	CGrid*		GetNeighbour( EGridDirection eDir );
	UINT32		GetMovableNeighbourCount( void ) const;
	CGrid*		GetMovableNeighbour( UINT32 uIndex );
protected:
	CTile*			m_pTile;
	CFVec2			m_fv2Corners[4];
	CFVec2			m_fv2Centre;
	bool			m_bFocusTile;
	CGrid*			m_pMoveableNeighbours[8];
	UINT32			m_uNumMoveableNeighbours;
};


#endif //_GRID_H__Included_1432030984

