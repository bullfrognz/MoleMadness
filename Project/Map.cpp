/**
 *	@file MobMoleMadness\Map.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Map.h"
#include "Tile.h"

#include "GDE/DataParser/DataParser.h"
#include "GDE/DataParser/DataParserManager.h"
#include <stdio.h>

CMap::CMap( void )
: m_pTiles( NULL )
, m_uNumTiles( 0 )
, m_uWidth( 0 )
, m_uHeight( 0 )
, m_pGrid( NULL )
{
}

CMap::~CMap( void )
{
	if ( m_pTiles )
	{
		delete[] m_pTiles;
	}
	if ( m_pGrid )
	{
		delete[] m_pGrid;
	}
}


bool CMap::Init( GDE::CDataParser* pParser )
{
	if ( false == pParser->GetValue( m_uNumTiles, "Main", "TileCount" ) )
	{
		pParser->Release();
		assert_now( "Unable to find a Main:TileCount parameter" );
		return false;
	}

	if ( false == pParser->GetValue( m_uWidth, "Main", "Width" ) )
	{
		pParser->Release();
		assert_now( "Unable to find a Main:Width parameter" );
		return false;
	}

	if ( false == pParser->GetValue( m_uHeight, "Main", "Height" ) )
	{
		pParser->Release();
		assert_now( "Unable to find a Main:Height parameter" );
		return false;
	}

	m_pGrid = new CGrid[m_uHeight*m_uWidth];

	m_pTiles = new CTile[m_uNumTiles];
	for ( UINT32 i=0; i<m_uNumTiles; i++ )
	{
		m_pTiles[i].Clear();
	}

	CTile* pTileLookup[256];
	for ( UINT32 i=0; i<256; i++ )
	{
		pTileLookup[i] = NULL;
	}

	for ( UINT32 i=0; i<m_uNumTiles; i++ )
	{
		if ( false == m_pTiles[i].Init( i, pParser ) )
		{
			assert_now( "Error parsing tile %d", i );
			pParser->Release();
			return false;
		}
		pTileLookup[m_pTiles[i].GetAlias()] = m_pTiles + i;
	}

	// now all the tiles are created we can read in the map!
	char strRowName[16];
	UINT8* strRow = new UINT8[m_uWidth+1];
	for ( UINT32 y=0; y<m_uHeight; y++ )
	{
		for ( UINT32 x=0; x<m_uWidth; x++ )
		{
			strRow[x] = 0;
		}
		sprintf_s( strRowName, "Row%02d", y );
		if ( false == pParser->GetString( reinterpret_cast<char*>(strRow), m_uWidth+1, "Map", strRowName ) )
		{
			assert_now( "Unable to find %s", strRow );
			for ( UINT32 x=0; x<m_uWidth; x++ )
			{
				GetGrid( x, y )->Init( m_pTiles );
			}
		}
		for ( UINT32 x=0; x<m_uWidth; x++ )
		{
			CTile* pTile = pTileLookup[strRow[x]];
			if ( NULL == pTile )
			{
				assert_now( "Invalid tile at %d,%d:%c", x, y, strRow[x] );
				GetGrid( x, y )->Init( m_pTiles );				
			} else
			{
				GetGrid( x, y )->Init( pTile );				
			}
		}
	}

	delete[] strRow;

	// now do the second pass init:
	for ( UINT32 i=0; i<m_uHeight*m_uWidth; i++ )
	{
		m_pGrid[i].InitNeighbours();
	}
	return true;
}

CGrid*	CMap::GetGrid( UINT32 uX, UINT32 uY )
{
	if ( uX < m_uWidth && uY < m_uHeight )
	{
		return m_pGrid + uX+m_uWidth*uY;
	}
	return NULL;
}

void	CMap::Draw( void )
{
	CGrid* pEnd = m_pGrid + m_uWidth*m_uHeight;
	for ( CGrid* pCur = m_pGrid; pCur!=pEnd; pCur++ )
	{
		pCur->Draw();
	}
}

bool	CMap::GetGridRef( CGrid* pGrid, SGridRef& Ref )
{
	UINT32 v = UINT32( pGrid - m_pGrid );
	UINT32 x = v%m_uWidth;
	UINT32 y = v/m_uWidth;
	if ( y<m_uHeight )
	{
		Ref.m_iX = (SINT32)x;
		Ref.m_iY = (SINT32)y;
		return true;
	}
	return false;
}

CGrid*	CMap::GetGridOnScreen( const CFVec2& vPos )
{
	UINT32 uX = UINT32( vPos.X() )/uTILE_WIDTH;
	UINT32 uY = UINT32( vPos.Y() )/uTILE_HEIGHT;
	return GetGrid( uX, uY );
}