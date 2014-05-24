/**
 *	@file GDE\GDE\Core\ResourceManagerBridge.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _RESOURCEMANAGERBRIDGE_H__Included_1554510870
#define _RESOURCEMANAGERBRIDGE_H__Included_1554510870

#include "GDE/Core/Bridge.h"
#include "GDE/Core/ResourceId.h"
#include "GDE/core/ResourceManagerImp.h"

namespace GDE
{
	/**
	 *	This template extends the bridge, specifically extended for common functionality required by resource manager classes.
	 *
	 *	The DLL ID should simply be the same one as is used for the manager that is responsible for owning the resource manager.
	 *	@see IResource.
	 *
	 *	TODO: use compile time polymorphism to define the way the things are stored rather than the hardcoded resource list as here.
	 */
	template <typename TManagerType, typename TImplementer, typename TResourceBridge, int iDLL_ID> class TResourceManagerBridge : public TBridge< TImplementer, iDLL_ID>
	{
	public:
		typedef TBridge< TImplementer, iDLL_ID> TParent;

		TResourceBridge*	LoadResource( const CResourceId& ResId );
		bool				ReleaseResource( TResourceBridge* pResource );
		TResourceBridge*	FindResouceById( const CResourceId& ResId );
		UINT32				GetResourceCount( void );
		UINT32				GetResourceMemory( void );
		TManagerType*		GetFinalManager( void ) { return reinterpret_cast<TManagerType*>(this); }

	protected:
		TResourceManagerBridge( void )
			: TParent()
			, m_pResourceList( NULL )
			, m_uResourceCount( 0 )
		{	// empty
		}

		TResourceManagerBridge( TImplementer* pImp )
		: TParent( pImp )
		, m_pResourceList( NULL )
		, m_uResourceCount( 0 )
		{	// empty
		}

		~TResourceManagerBridge( void )
		{
			char ManagerName[5];
			DLLIdToString( iDLL_ID, ManagerName );
			assert_msg( 0 ==m_uResourceCount, "Manager %s has %d resources remaining\n", ManagerName, m_uResourceCount );
		}
		TResourceBridge*	m_pResourceList;		//!< Pointer to a list of resources.
		UINT32				m_uResourceCount;		//!< The number of resources.
	};

	/////////////////////////////////////////////////////////////////////////
	// Inline implementation of the above

	/**
	 *	Load a resource or returns an existing one if the Id is already loaded.
	 *
	 *	@param strFileName	The filename of the resource to load.
	 *	@param strId		The Id to use for this resource.
	 *	@param pTexture		Is filled in with the resource, if load succeeds, otherwise is left alone.
	 *	@return				true on success, otherwise false.
	 */
	template <typename TManagerType, typename TImplementer, typename TResourceBridge, int iDLL_ID> 
	TResourceBridge* TResourceManagerBridge<TManagerType,TImplementer,TResourceBridge,iDLL_ID>::LoadResource( 
			const CResourceId&	ResId )
	{
		TResourceBridge* pResource = FindResouceById( ResId );
		if ( pResource )
		{
			pResource->AddRef();
			return pResource;
		}
		if ( NULL != m_pImplementer )
		{
			typename TResourceBridge::TImplementer * pNakedResource = NULL;
			// nasty cast ensures that we can use this inline as all Implementers must be of the TIResourceManagerImp type.
			if ( reinterpret_cast< TIResourceManagerImp< TResourceBridge::TImplementer > * >(m_pImplementer)->LoadResource( ResId.GetFileName(), pNakedResource ) )
			{
				++m_uResourceCount;
				m_pResourceList = new TResourceBridge( pNakedResource, ResId, /*reinterpret_cast<TResourceBridge::TManagerBase*>*/(this), m_pResourceList );
				return m_pResourceList;
			}
		}
		return false;	// no Implementer present.
	}

	/**
	 *	Release a given resource.
	 *
	 *	@param pResource	The resource to release.
	 */
	template <typename TManagerType, typename TImplementer, typename TResourceBridge, int iDLL_ID> 
	bool TResourceManagerBridge<TManagerType,TImplementer,TResourceBridge,iDLL_ID>::ReleaseResource( 
		TResourceBridge* pResource )
	{
		if ( NULL == pResource )
		{
			assert_now( "should not release NULL resource" );
			return false;
		}
		// remove the item from the list:
		if ( pResource == m_pResourceList )
		{
			m_pResourceList = pResource->m_pNext;
		} else
		{
			TResourceBridge* pCur = m_pResourceList;
			for ( ;
				NULL != pCur;
				pCur = pCur->m_pNext )
			{
				if ( pCur->m_pNext == pResource )
				{
					pCur->m_pNext = pResource->m_pNext;
					break;
				}
			}
			if ( NULL == pCur )
			{
				assert_now( "This resource is not in the manager's list" );
				return false;
			}
		}

		// great, now it's removed from the list we can free it:
		reinterpret_cast< TIResourceManagerImp< TResourceBridge::TImplementer > * >(m_pImplementer)->FreeResource( pResource->m_pImplementer );
		delete pResource;
		--m_uResourceCount;
		return true;
	}

	/**
 	 *	Find a resource, based on it's Id.
	 *
	 *	Adds a reference to the resource if it is found.
	 *`
	 *	@param ResId		The unique identifier for this resource.
	 *	@return				The found resource or NULL if not found.
	 */
	template <typename TManagerType, typename TImplementer, typename TResourceBridge, int iDLL_ID> 
	TResourceBridge* TResourceManagerBridge<TManagerType,TImplementer,TResourceBridge,iDLL_ID>::FindResouceById( 
		const CResourceId& ResId )
	{
		for ( TResourceBridge* pCur = m_pResourceList;
				NULL != pCur;
				pCur = pCur->m_pNext )
		{
			if ( pCur->GetId() == ResId )
			{
				pCur->AddRef();
				return pCur;
			}
		}
		return NULL;
	}

} // end namespace GDE

#endif //_RESOURCEMANAGERBRIDGE_H__Included_1554510870

