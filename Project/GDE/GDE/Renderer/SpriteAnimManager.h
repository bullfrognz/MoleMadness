/**
 *	@file GDE\GDE\Renderer\SpriteAnimManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SPRITEANIMMANAGER_H__Included_1704528597
#define _SPRITEANIMMANAGER_H__Included_1704528597


#include "GDE/Core/ResourceManagerBridge.h"

namespace GDE
{
	class CSpriteManager;
	class ISpriteAnimManagerImp;
	class CSpriteAnim;
	class CTexture;
	struct SFRect;

	class CSpriteAnimManager : public TResourceManagerBridge< CSpriteAnimManager, ISpriteAnimManagerImp, CSpriteAnim, CREATE_DLL_ID( 'S','P','R','A' ) >
	{
	public:
		typedef TResourceManagerBridge< CSpriteAnimManager, ISpriteAnimManagerImp, CSpriteAnim, CREATE_DLL_ID( 'S','P','R','A' ) > TParent;

		CSpriteAnimManager( void )
			: TParent()
			, m_pSpriteManager( NULL )
		{	
		}
		CSpriteManager*	GetSpriteManager( void ) { return m_pSpriteManager; }

		bool			LoadDLL( const char* strDLLName, CSpriteManager* pSpriteManager );
		CSpriteAnim*	CreateAnim(
			const CResourceId&	ResID, 
			UINT32				uFrameCount,
			FLOAT32				fFrameRate,
			CTexture*			pTexture,
			const SFRect&		sTextureRect );
	protected:
		using TParent::LoadDLL;	// no loading the DLL with that superclass default implementation. Sorry.
		CSpriteManager* m_pSpriteManager;	//!< The manager used for the sprites.
	};
}	// end namespace GDE

#endif //_SPRITEANIMMANAGER_H__Included_1704528597

