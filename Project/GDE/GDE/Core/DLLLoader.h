/**
 *	@file GDE\GDE\Core\DLLLoader.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _DLLLOADER_H__Included_871020847
#define _DLLLOADER_H__Included_871020847

namespace GDE
{

class CDLLWrapper;

/**
 *	This class deals with loading and saving the DLL.
 */
class CDLLLoader
{
public:
	CDLLLoader( void );
	template <typename tDLL> bool GetDLLWrapper( const char* DLLName, int iDLL_ID, tDLL* &pLoadedDLL )
	{
		if ( LoadDLLInterface( DLLName, iDLL_ID ) )
		{
			pLoadedDLL = (tDLL*)m_pDLLInstance;
			return true;
		}
		return false;
	}

	bool ShutdownDLL( int iDLL_ID );
	~CDLLLoader( void );
protected:
	bool LoadDLLInterface( const char* DLLName, int iDLL_ID );

	CDLLWrapper*	m_pDLL;
	void*			m_pDLLInstance;
};

}// end namespace GDE

#endif //_DLLLOADER_H__Included_871020847

