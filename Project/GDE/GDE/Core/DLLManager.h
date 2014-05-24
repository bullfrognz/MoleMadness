/**
 *	@file GDE\GDE\Core\DLLManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _DLLMANAGER_H__Included_913813592
#define _DLLMANAGER_H__Included_913813592

#include "ResourceManagerBridge.h"

namespace GDE
{
	class CDLLWrapper;

	class CDLLManager
	{
	public:
		static CDLLManager& GetInstance( void );	//Meyers singleton.

		CDLLWrapper* GetDLL( const char* strFileName );
	protected:
		friend class CDLLWrapper;	// allow the DLL wrapper to call the ReleaseDLL function.
		bool	ReleaseDLL( CDLLWrapper* pDLL );

		CDLLManager( void ) {};	// empty constructor - singleton.

		// note there is a container, however, it is simply file static to hide it
		// and reduce dependancies.
	};

} // end namespace GDE

#endif //_DLLMANAGER_H__Included_913813592

