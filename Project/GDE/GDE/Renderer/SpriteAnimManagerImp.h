#ifndef _SPRITEANIMMANAGERIMP_H__Included_1973899618
#define _SPRITEANIMMANAGERIMP_H__Included_1973899618

#include "GDE/Core/ResourceManagerImp.h"

namespace GDE
{
	class	ISpriteAnimImp;
	class	CSpriteManager;
	class	CTexture;
	struct	SFRect;

	/**
	 *	The sprite animation manager Implementer.
	 */
	class ISpriteAnimManagerImp : public TIResourceManagerImp<ISpriteAnimImp>
	{
	public:
		/**
		 *	Set the sprite manager for this manager to create the animations with.
		 *
		 *	@param pSpriteManager	The sprite mananger.
		 *	@return					false if the sprite manager isn't supported.
		 */
		virtual bool SetSpriteManager( CSpriteManager* pSpriteManager ) = 0;

		/**
		 *	Create a sprite animation.
		 *	After the first frame, subsequent frames must come to the left in the texure.
		 *	When there is no more space available to the left of the previous frame
		 *	the next frame starts directly below and to the very left of the texture page.
		 *
		 *	@param uFrameCount		The number of frames in the animation.
		 *	@param fFrameRate		The number of frames per second to play the animation at.
		 *	@param pTexture			The texture to use for the sprite.
		 *	@param sTextureRect		The rectangle of the texture the sprite uses - in texels, for the first frame.
		 *	@return					The sprite animation (on success). Refcount will be one!
		 */
		virtual ISpriteAnimImp* CreateAnim(
			UINT32				uFrameCount,
			FLOAT32				fFrameRate,
			CTexture*			pTexture,
			const SFRect&		sTextureRect ) = 0;
	};

}	// end namespace GDE

#endif //_SPRITEANIMMANAGERIMP_H__Included_1973899618

