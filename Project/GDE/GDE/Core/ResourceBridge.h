/**
 *	@file GDE\GDE\Core\ResourceBridge.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _RESOURCEBRIDGE_H__Included_310043916
#define _RESOURCEBRIDGE_H__Included_310043916

#include "GDE/Core/Bridge.h"
#include "GDE/Core/ResourceId.h"

namespace GDE
{
	// forward reference a template?
	template <typename TManagerType, typename TImplementer, typename TResourceBridge, int iDLL_ID> class TResourceManagerBridge;

	/**
	 *	This template extends the bridge, specifically extended for common functionality required by resources.
	 *	So reference counted etc.
	 *	The DLL ID should simply be the same one as is used for the manager that is responsible for the resource.
	 *	@see IResource.
	 */
	template <typename TManagerType,typename TImplementer, int iDLL_ID> 
	class TResourceBridge : public TBridge< TImplementer, iDLL_ID>
	{
	public:
		typedef TImplementer TImplementer;
		typedef TBridge< TImplementer, iDLL_ID> TParent;
		typedef TResourceBridge<TManagerType, TImplementer, iDLL_ID> TThis;
		typedef TResourceManagerBridge<TManagerType, TImplementer, TThis, iDLL_ID> TManagerBase;
//		typedef typename TManagerType::TParent TMangerBase;

		void				AddRef( void );	
		bool				Release( void );
		UINT32				GetRefCount( void ) const;
		const CResourceId&	GetId( void ) const;
	protected:
		// allow the manager to get access to the internals of this - specifically setting the Implementer
		// as the bridge to it's manager returns them.
		friend TManagerBase;
//		friend typename TManagerType::TParent;

		/**
		 *	Constructor - only used by the manager, needs explicitly recreating for each child in a protected manner.
		 */
		TResourceBridge( TImplementer* pImplementer, const CResourceId& Id, TManagerBase* pManager, TManagerType* pNext )
		: TParent( pImplementer )
		, m_uRefCount( 1 )
		, m_pNext( pNext )
		, m_pManager( pManager )
		, m_Id( Id )
		{	// empty.
		}

		UINT32					m_uRefCount;	//!< Current reference count.
		TManagerType*			m_pNext;		//!< The next resource in the linked list of all resources of this type.
		TManagerBase*			m_pManager;		//!< The manager that owns this.
		CResourceId				m_Id;			//!< The Id of this resource.

	private:
		// hide away the following parent class implementation as it is inappropriate.
		using TParent::LoadDLL;
		using TParent::ReleaseDLLImplementer;

	};

	////////////////////////////////////////////////////
	// inline implementation of the above:

	/**
	 *	Add a reference to this resource.
	 */
	template <typename TManagerType,typename TImplementer, int iDLL_ID> 
	void TResourceBridge<TManagerType,TImplementer,iDLL_ID>::AddRef( void )
	{
		if ( NULL != m_pImplementer )
		{
			++m_uRefCount;
		}
	}

	/**
	 *	Release this resource.
	 *
	 *	@return true if this was the last reference count freed and so the resource is finished with, or if the bridge is not implemented.
	 */
	template <typename TManagerType, typename TImplementer, int iDLL_ID> 
	bool TResourceBridge<TManagerType,TImplementer,iDLL_ID>::Release( void )
	{
		if ( NULL != m_pImplementer )
		{
			--m_uRefCount;
			if ( 0 == m_uRefCount )
			{
				m_pManager->ReleaseResource( this );
				return true;
			}
			return false;
		} else
		{
			delete this;
			return true;
		}
	}

	/**
	 *	@return the current reference count for the resource or zero if the bridge is not implemented.
	 */
	template <typename TManagerType, typename TImplementer, int iDLL_ID> 
	UINT32 TResourceBridge<TManagerType,TImplementer,iDLL_ID>::GetRefCount( void ) const
	{
		return m_uRefCount;
	}

	/**
	 *	@return The resource Id of this resouce
	 */
	template <typename TManagerType, typename TImplementer, int iDLL_ID> 
	const CResourceId&	TResourceBridge<TManagerType,TImplementer,iDLL_ID>::GetId( void ) const
	{
		return m_Id;
	}


};
#endif //_RESOURCEBRIDGE_H__Included_310043916

