 /**
 *	@file GDE\GDE\Core\ResourceManagerImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _RESOURCEMANAGERIMP_H__Included_1524369068
#define _RESOURCEMANAGERIMP_H__Included_1524369068

#include "Implementer.h"

namespace GDE
{

	/**
	 *	A generic resource manager implementation.
	 */
	template <typename TResource> class TIResourceManagerImp : public IImplementer
	{
	public:
		 virtual bool		LoadResource( const char* strFileName, TResource* &pResource ) = 0;
		 virtual bool		FreeResource( TResource* pResource ) = 0;
	};

} // end namespace GDE

#endif //_RESOURCEMANAGERIMP_H__Included_1524369068

