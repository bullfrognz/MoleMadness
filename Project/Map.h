/**
 *	@file MobMoleMadness\Map.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _MAP_H__Included_54364539
#define _MAP_H__Included_54364539

#include "Grid.h" // TODO: remove header chaining, can be by reference?
#include "FVector2.h"

namespace GDE
{
	class CDataParser;	
};


class CMap
{
public:
	CMap( void );
	~CMap( void );
	bool	Init( GDE::CDataParser* pParser );
	CGrid*	GetGrid( UINT32 uX, UINT32 uY );
	void	Draw( void );
	bool	GetGridRef( CGrid* pGrid, SGridRef& Ref );
	CGrid*	GetGridOnScreen( const CFVec2& vPos );
protected:
	CTile*	m_pTiles;				//!< All the tiles available in the game.
	UINT32	m_uNumTiles;			//!< How many tiles are there?
	UINT32	m_uWidth;				//!< Width of the map.
	UINT32	m_uHeight;				//!< Height of the map.
	CGrid*	m_pGrid;
};
#endif //_MAP_H__Included_54364539

