/**
 *	@file MobMoleMadness\Tile.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "Tile.h"
#include "MMMContext.h"
#include "GDE/Core/Rect.h"
#include "GDE/Renderer/Sprite.h"
#include "GDE/Renderer/SpriteManager.h"
#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"
#include "GDE/DataParser/DataParser.h"

//#include <string.h>
#include <stdio.h>
const static UINT32 MAX_PATH = 260;

CTile::~CTile( void )
{
	if ( m_pSprite )
	{
		m_pSprite->Release();
	}
}

void CTile::Clear( void )
{
	m_pSprite = NULL;
	m_iCost = -1;
	m_cAlias = 0;
}


/**
 *	@param uId		The identifier for this tile (zero based integer index).
 *	@param pParser	The parser to read the data from.
 */
bool CTile::Init( GDE::UINT32 uId, GDE::CDataParser* pParser )
{

	char strSection[16];
	sprintf_s( strSection, "Tile%d", uId );
	if ( false == pParser->GetValue( m_iCost, strSection, "Cost" ) )
	{
		assert_now( "Unable to find Cost token for %s", strSection );
		return false;
	}
	if ( false == pParser->GetString( m_strName, uMAX_NAME, strSection, "Name" ) )
	{
		assert_now( "Unable to find Name token for %s", strSection );
		return false;
	}

	char strSpriteImageName[MAX_PATH];
	if ( false == pParser->GetString( strSpriteImageName, 2, strSection, "Alias" ) )
	{
		assert_now( "Unable to find Alias token for %s", strSection );
		return false;
	}
	m_cAlias = strSpriteImageName[0];

	if ( false == pParser->GetString( strSpriteImageName, MAX_PATH, strSection, "Image" ) )
	{
		assert_now( "Unable to find Image token for %s", strSection );
		return false;
	}
	// now load the sprite.

	GDE::CTexture* pTexture = CMMMContext::GetInstance().GetRenderer()->GetTextureManager().LoadResource( strSpriteImageName );
	if ( NULL == pTexture )
	{
		assert_now( "Unable to find Image \"%s\" section for %s", strSpriteImageName, strSection );
	}
	GDE::SFRect TexRect;
	TexRect.m_fX = 0.0f;
	TexRect.m_fY = 0.0f;
	TexRect.m_fWidth = (float)pTexture->GetWidth();
	TexRect.m_fHeight = (float)pTexture->GetHeight();

	m_pSprite = CMMMContext::GetInstance().GetSpriteM()->CreateSprite( GDE::CResourceId( strSection ), pTexture, TexRect );
	pTexture->Release();	// now held inside the sprite.
	return true;
}

bool CTile::Draw( UINT32 uX, UINT32 uY )
{
	float fX = float( uX*uTILE_WIDTH );
	float fY = float( uY*uTILE_HEIGHT );
	m_pSprite->Render( fX, fY );
	return true;
}
