/**
 *	@file GDE\GDE\Renderer\Texture.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TEXTURE_H__Included_278708145
#define _TEXTURE_H__Included_278708145

#include "GDE/Core/ResourceBridge.h"

namespace GDE
{
	class ITextureImp;
	class CTextureManager;
	class ITextureManagerImp;
	class CResourceId;
	struct SIRect;

	template <typename TManagerType, typename TImplementer, typename TResourceBridge, int iDLL_ID> class TResourceManagerBridge;

	class CTexture : public TResourceBridge< CTexture, ITextureImp, CREATE_DLL_ID( 'R','N','D','R') >
	{
	public:
		typedef CTexture TThis;
		typedef ITextureImp TImplementer;	//!< All bridges need this typedef in the final type.
		typedef TResourceBridge< CTexture, ITextureImp, CREATE_DLL_ID( 'R','N','D','R') > TParent;
		typedef TResourceManagerBridge< CTextureManager, ITextureManagerImp, TThis, CREATE_DLL_ID( 'R','N','D','R') > TManagerBase;
//		typedef TParent::TManagerBase TManagerBase;

		bool		Apply( UINT32 uTextureStage );
		UINT32		GetWidth( void ) const;
		UINT32		GetHeight( void ) const;
		bool		LockTexels( UINT32* &pTexels, UINT32& uPitch, bool bReadOnly = true, const SIRect* pRegion = NULL );
		bool		UnlockTexels( void );
	protected:
		// need to be a special friend to allow the manager access to create these:
		friend TManagerBase;
		friend CTextureManager;
		/**
		 *	Constructor - only used by the manager, has to be overloaded though explicitly
		 */
		CTexture( ITextureImp* pImplementer, const CResourceId& Id, TManagerBase* pManager, CTexture* pNext )
		: TParent( pImplementer, Id,  reinterpret_cast<TParent::TManagerBase*>(pManager), pNext ) // TODO: don't know why the cast, should be the same type.
		{	// empty.
		}
	};
} // end namespace GDE

#endif //_TEXTURE_H__Included_278708145

