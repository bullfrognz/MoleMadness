/**
 *	@file GDE\GDE\Core\Bridge.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _BRIDGE_H__Included_960497723
#define _BRIDGE_H__Included_960497723

#include "DLLLoader.h"
#include "DLLInterface.h"
#include "Implementer.h"

namespace GDE
{
class CDLLLoader;

/**
 *	A template class creating a Bridge design pattern to use with our managers.
 *	Each manager should inheret from this class.
 *
 */
template <typename TImplementer, int iDLL_ID> class TBridge
{
public:
	~TBridge( void )
	{
		if ( m_pDLLLoader )
		{
			m_pDLLLoader->ShutdownDLL( iDLL_ID );
			delete m_pDLLLoader;
		}
	}
	void ReleaseDLLImplementer( void )
	{
		if ( m_pDLLLoader )
		{
			m_pDLLLoader->ShutdownDLL( iDLL_ID );
			delete m_pDLLLoader;
			m_pImplementer = NULL;
		} 
	}

	const CLibraryDescription&	GetImplementerDesc( void ) const;
	bool						IsImplemented( void )  const;
	int							GetDLL_ID( void ) const { return iDLL_ID; }
	bool						LoadDLL( const char* strDLLName );
	TImplementer*				GetImplementer( void ) { return m_pImplementer; }
protected:
	TBridge( void )
	: m_pImplementer( NULL )
	, m_pDLLLoader( NULL )
	{ // empty
	}

	TBridge( TImplementer* pImplementer )
	: m_pImplementer( pImplementer )
	, m_pDLLLoader( NULL )
	{	// empty
	}

	TImplementer*	m_pImplementer;
	CDLLLoader*		m_pDLLLoader;
};

#define _BRIDGE_RETURN_FALSE_WITH_NO_Implementer() if ( NULL == m_pImplementer ) return false;
#define _BRIDGE_ASSERT_AND_RETURN_FALSE_WITH_NO_Implementer() if ( NULL == m_pImplementer ) { assert_now( "There is no Implementer for this bridge" ); return false; }
// Inline (as a template) member function definitions:

/**
 *	Each Implementer of bridge functionality should have a unique description.
 *
 *	@return The Implementer of this bridge's unique textual description.
 */
template <typename TImplementer, int iDLL_ID> const CLibraryDescription& TBridge<TImplementer,iDLL_ID>::GetImplementerDesc( void ) const
{
	if ( NULL != m_pImplementer )
	{
		// nasty cast to ensure something works.
		return reinterpret_cast<IImplementer*>(m_pImplementer)->GetDesc();
	} else
	{
		return gs_NULLLibraryDesc;
	}
}

/**
 *	@return Is this bridge attached to an implementation?
 */
template <typename TImplementer, int iDLL_ID> bool TBridge<TImplementer,iDLL_ID>::IsImplemented() const
{
	return ( NULL != m_pImplementer );
}

template <typename TImplementer, int iDLL_ID> bool TBridge<TImplementer,iDLL_ID>::LoadDLL( const char* strDLLName )
{
	if ( NULL == m_pDLLLoader )
	{
		m_pDLLLoader = new CDLLLoader();
	}
	return m_pDLLLoader->GetDLLWrapper( strDLLName, iDLL_ID, m_pImplementer );
}


} // end namespace GDE

#endif //_BRIDGE_H__Included_960497723

