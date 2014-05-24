/**
 *	@file GDE\GDE\Renderer\Sprite.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SPRITE_H__Included_328911452
#define _SPRITE_H__Included_328911452

#include "GDE/Core/ResourceBridge.h"
#include "GDE/Renderer/FontConstants.h"

namespace GDE
{

class ISpriteImp;
class CSpriteManager;
class ISpriteManagerImp;
class CTexture;
struct SFRect;

/**
 *	The font class.
 */
class CSprite : public TResourceBridge< CSprite, ISpriteImp, CREATE_DLL_ID( 'S','P','R','T' ) >
{
public:

	typedef CSprite TThis;
	typedef ISpriteImp TImplementer;	//!< All bridges need this typedef in the final type.
	typedef TResourceBridge< CSprite, ISpriteImp, CREATE_DLL_ID( 'S','P','R','T') > TParent;
	typedef TResourceManagerBridge< CSpriteManager, ISpriteManagerImp, TThis, CREATE_DLL_ID( 'S','P','R','T' ) > TManagerBase;

	bool Render( FLOAT32 fX, FLOAT32 fY );
	bool RenderEx( FLOAT32 fX, FLOAT32 fY, FLOAT32 fRot, FLOAT32 fXScale, FLOAT32 fYScale );

	bool SetColour( UINT32 colSprite );
	UINT32 GetColour( void ) const;

	bool SetTexture( CTexture* pTexture );
	CTexture* GetTexture( void ) const;

	bool SetTextureRect( const SFRect& Rect );
	bool GetTextureRect( SFRect& Rect ) const;

	bool SetHotSpot( FLOAT32 fX, FLOAT32 fY );
	bool GetHotSpot( FLOAT32& fX, FLOAT32& fY ) const;

	FLOAT32 GetWidth( void ) const;
	FLOAT32 GetHeight( void ) const;
protected:
	// need to be a special friend to allow the manager access to create these:
	friend TManagerBase;
	friend CSpriteManager;	// also the sprite manager needs to do this!
	/**
	 *	Constructor - only used by the manager, has to be overloaded though explicitly
	 */
	CSprite( ISpriteImp* pImplementer, const CResourceId& Id, TManagerBase* pManager, CSprite* pNext )
	: TParent( pImplementer, Id,  reinterpret_cast<TParent::TManagerBase*>(pManager), pNext ) // TODO: don't know why the cast, should be the same type.
	{	// empty.
	}
};

} // end namespace GDE

#endif //_SPRITE_H__Included_328911452

