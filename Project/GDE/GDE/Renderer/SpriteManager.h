/**
 *	@file GDE\GDE\Renderer\SpriteManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SPRITEMANAGER_H__Included_1262188081
#define _SPRITEMANAGER_H__Included_1262188081

#include "GDE/Core/ResourceManagerBridge.h"

namespace GDE
{
	class CRenderer;
	class ISpriteManagerImp;
	class CSprite;
	class CTexture;
	struct SFRect;

	class CSpriteManager : public TResourceManagerBridge< CSpriteManager, ISpriteManagerImp, CSprite, CREATE_DLL_ID( 'S','P','R','T' ) >
	{
	public:
		typedef TResourceManagerBridge< CSpriteManager, ISpriteManagerImp, CSprite, CREATE_DLL_ID( 'S','P','R','T' ) > TParent;

		CSpriteManager( void )
			: TParent()
			, m_pRenderer( NULL )
		{	
		}
		CRenderer* GetRenderer( void ) { return m_pRenderer; }

		bool	LoadDLL( const char* strDLLName, CRenderer* pRenderer );
		CSprite* CreateSprite( const CResourceId& ResID, CTexture* pTexture, const SFRect& sTextureRect );
	protected:
		using TParent::LoadDLL;	// no loading the DLL with that superclass default implementation. Sorry.
		CRenderer* m_pRenderer;	//!< The renderer used for the Sprites.
	};
}	// end namespace GDE

#endif //_SPRITEMANAGER_H__Included_1262188081

