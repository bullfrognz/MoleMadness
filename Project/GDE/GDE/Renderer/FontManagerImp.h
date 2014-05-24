/**
 *	@file GDE\GDE\Renderer\FontManagerImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _FONTMANAGERIMP_H__Included_31091421
#define _FONTMANAGERIMP_H__Included_31091421

#include "GDE/Core/ResourceManagerImp.h"

namespace GDE
{
	class IFontImp;
	class CRenderer;

	/**
	 *	The Font manager Implementer.
	 */
	class IFontManagerImp : public TIResourceManagerImp<IFontImp>
	{
	public:
		/**
		 *	Set the renderer for this manager.
		 *
		 *	@param pRenderer	The renderer.
		 *	@return				false if the renderer isn't supported.
		 */
		virtual bool SetRenderer( CRenderer* pRenderer ) = 0;
	};

}	// end namespace GDE

#endif //_FONTMANAGERIMP_H__Included_31091421

