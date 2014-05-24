/**
 *	@file GDE\GDE\Renderer\TextureManagerImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TEXTUREMANAGERIMP_H__Included_354835106
#define _TEXTUREMANAGERIMP_H__Included_354835106

#include "GDE/Core/ResourceManagerImp.h"

namespace GDE
{
	class ITextureImp;

	/**
	 *	The texture manager Implementer.
	 */
	class ITextureManagerImp : public TIResourceManagerImp<ITextureImp>
	{
	public:
		/**
		 *	@return A NULL texture implementation, which when bound to the device means untextured or to disable texturing.
		 */
		virtual bool GetNULLTexture( ITextureImp*& pNULLTexture ) = 0;
	};

}	// end namespace GDE

#endif //_TEXTUREMANAGERIMP_H__Included_354835106

