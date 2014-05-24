/**
 *	@file GDE\GDE\Core\DLLWrapper.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _DLLWRAPPER_H__Included_315631676
#define _DLLWRAPPER_H__Included_315631676

namespace GDE
{
	/**
	 *	This class is used to wrap a DLL instance.
	 *	It is reference counted so it can be deleted once finished with.
	 */
	class CDLLWrapper
	{
	public:
		const static UINT32	uMAX_DLL_NAME = 260;	//!< Maximum number of characters in the DLL name.

		typedef void* (__cdecl*DLL_GetInterface)( int DLL_ID );
		typedef	bool (__cdecl*DLL_ShutdownInterface)( int DLL_ID, void* pBase );

		CDLLWrapper( const char* DLLName );
		~CDLLWrapper( void );

		// reference counting
		void		AddRef( void );	
		bool		Release( void );
		UINT32		GetRefCount( void ) const;	

		// Accessing a DLL interface
		const char*	GetFileName( void ) const;
		bool		DLLExists( void ) const;
		bool		GetDLLInterface( int iDLL_ID, void*& pInterface );
		bool		FreeDLLInterface( int iDLL_ID, void* pInterface );
	private:
		UINT32					m_uRefCount;					//!< How many libraries are loaded through this DLL - starts at one after construction.
		char					m_strFileName[uMAX_DLL_NAME];	//!< The filename for the DLL.
		void*					m_hModule;						//!< The windows module - to remove nasty dependancies it's a void*
		DLL_GetInterface		m_cbGetInterface;				//!< DLL function to get the library interface.
		DLL_ShutdownInterface	m_cbShutdownInterface;			//!< DLL function to shut down the interface.
	};
}	// end namespace GDE

#endif //_DLLWRAPPER_H__Included_315631676

