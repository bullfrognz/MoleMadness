/**
 *	@file GDE\GDE\Renderer\SpriteManagerImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SPRITEMANAGERIMP_H__Included_350048382
#define _SPRITEMANAGERIMP_H__Included_350048382

#include "GDE/Core/ResourceManagerImp.h"

namespace GDE
{
	class	ISpriteImp;
	class	CRenderer;
	class	CTexture;
	struct	SFRect;

	/**
	 *	The Sprite manager Implementer.
	 */
	class ISpriteManagerImp : public TIResourceManagerImp<ISpriteImp>
	{
	public:
		/**
		 *	Set the renderer for this manager and its sprites to work with.
		 *
		 *	@param pRenderer	The renderer.
		 *	@return				false if the renderer isn't supported.
		 */
		virtual bool SetRenderer( CRenderer* pRenderer ) = 0;

		/**
		 *	Create an sprite.
		 *
		 *	@param pTexture		The texture for the sprite.
		 *	@param sTextureRect	A rectange defining the sprite's texture coordinates - as texels.
		 *	@return				The sprite or NULL on fail.
		 */
		virtual ISpriteImp* CreateSprite( CTexture* pTexture, const SFRect& sTextureRect ) = 0;
	};

}	// end namespace GDE

#endif //_SPRITEMANAGERIMP_H__Included_350048382

