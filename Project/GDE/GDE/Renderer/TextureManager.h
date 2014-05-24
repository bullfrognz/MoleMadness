/**
 *	@file GDE\GDE\Renderer\TextureManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TEXTUREMANAGER_H__Included_1035357785
#define _TEXTUREMANAGER_H__Included_1035357785

#include "GDE/Core/ResourceManagerBridge.h"

namespace GDE
{
	class ITextureManagerImp;
	class CTexture;

	class CTextureManager : public TResourceManagerBridge< CTextureManager, ITextureManagerImp, CTexture, CREATE_DLL_ID( 'R','N','D','R') >
	{
	public:
		typedef TResourceManagerBridge< CTextureManager, ITextureManagerImp, CTexture, CREATE_DLL_ID( 'R','N','D','R') > TParent;
		// Most implementation is carried out by the generic manager bridge construct...
		CTexture* GetNULLTexture( void );
		~CTextureManager( void );
	protected:
		friend class CRenderer;	// allow the renderer to call the constructor.
		CTextureManager( ITextureManagerImp* pImp );

		CTexture*	m_pNULLTexture;
	};

}	// end namespace GDE

#endif //_TEXTUREMANAGER_H__Included_1035357785

