/**
 *	@file MobMoleMadness\Tile.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TILE_H__Included_1492865839
#define _TILE_H__Included_1492865839

const static UINT32 uTILE_WIDTH = 32;
const static UINT32 uTILE_HEIGHT = 32;

namespace GDE
{
	class CSprite;
	class CDataParser;
};

class CTile
{
public:
	~CTile( void );
	void Clear( void );
	bool Init( UINT32 uId, GDE::CDataParser* pParser );
	bool Draw( UINT32 uX, UINT32 uY );
	UINT8 GetAlias( void ) { return (UINT8)m_cAlias; }
	SINT32		GetCost( void ) const { return m_iCost; }
	const char*	GetName( void ) const { return m_strName; }
protected:
	const static UINT32	uMAX_NAME = 16;
	// protected constructor, should only be built.
	//CTile( GDE::CSprite* pSprite, SINT32 iCost );
	GDE::CSprite*	m_pSprite;
	SINT32			m_iCost;
	char			m_cAlias;
	char			m_strName[uMAX_NAME];
};
#endif //_TILE_H__Included_1492865839

