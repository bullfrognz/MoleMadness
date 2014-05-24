/**
 *	@file GDE\GDE\Renderer\FontManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _FONTMANAGER_H__Included_2147364214
#define _FONTMANAGER_H__Included_2147364214

#include "GDE/Core/ResourceManagerBridge.h"

namespace GDE
{

	class CRenderer;
	class IFontManagerImp;
	class CFont;

	class CFontManager : public TResourceManagerBridge< CFontManager, IFontManagerImp, CFont, CREATE_DLL_ID( 'F','O','N','T') >
	{
	public:
		typedef TResourceManagerBridge< CFontManager, IFontManagerImp, CFont, CREATE_DLL_ID( 'F','O','N','T') > TParent;

		CFontManager( void )
			: TParent()
			, m_pRenderer( NULL )
		{	
		}
		CRenderer* GetRenderer( void ) { return m_pRenderer; }

		bool	LoadDLL( const char* strDLLName, CRenderer* pRenderer );
	protected:
		using TParent::LoadDLL;	// no loading the DLL with that superclass default implementation. Sorry.
		CRenderer* m_pRenderer;	//!< The renderer used for the fonts.
	};
}	// end namespace GDE



#endif //_FONTMANAGER_H__Included_2147364214

