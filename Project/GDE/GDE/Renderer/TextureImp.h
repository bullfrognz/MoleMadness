/**
 *	@file GDE\GDE\Renderer\TextureImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TEXTUREIMP_H__Included_1134709354
#define _TEXTUREIMP_H__Included_1134709354

#include "GDE/Core/Resource.h"

namespace GDE
{
	struct SIRect;

	class ITextureImp : public IResource
	{
	public:
		/**
		 *	Apply the texture to be rendered on subsequent primitives.
		 *	@param uTextureStage	The texture stage to apply to.
		 */
		virtual bool	Apply( UINT32 uTextureStage ) = 0;
		/**
		 *	@return The width, in texels, of the texture or zero on error.
		 */
		virtual UINT32	GetWidth( void ) const = 0;
		/**
		 *	@return The height, in texels, of the texture or zero on error.
		 */
		virtual UINT32	GetHeight( void ) const = 0;

		/**
		 *	Lock the texels as UINT32 colour values in an array. All locked textures must be subsequently
		 *	unlocked. @see UnlockTexels.
		 *
		 *	The first value in the array is the top left texel, subsequent values move to the right.
		 *	Each row starts at pTexels[uPitch*uRow];
		 *
		 *	@param pTexels		Filled in with a pointer to the texels.
		 *	@param uPitch		Filled in with the pitch (offset between subsequent rows of texels).
		 *	@param bReadOnly	Are the texels read only? Otherwise changes will be updated to the texture.
		 *	@param pRegion		Optional sub region to lock.
		 *	@return				Success. Some textures (compressed for example) may not support locking.
		 */
		virtual bool	LockTexels( UINT32* &pTexels, UINT32& uPitch, bool bReadOnly = true, const SIRect* pRegion = NULL ) = 0;

		/**
		 *	Unlock the texture's texels.
		 *	If the texture was not locked read only then this will update the texture with the new values filled in.\
		 *	@return Success.
		 */
		virtual bool	UnlockTexels( void ) = 0;
	};
} // end namespace GDE

#endif //_TEXTUREIMP_H__Included_1134709354

