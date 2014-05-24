/**
 *	@file GDE\GDE\Core\Resource.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _RESOURCE_H__Included_355108999
#define _RESOURCE_H__Included_355108999

namespace GDE
{
	/** 
	 *	Superclass for resource implementations.
	 *	Handles reference counting information.
	 */
	class IResource
	{
	public:
		// TODO: remove/clean this up!
		///**
		// *	Add a reference to this resource.
		// */
		//virtual void		AddRef( void ) = 0;	
		///**
		// *	Release this resource.
		// *
		// *	@return true if this was the last reference count freed and so the resource is finished with.
		// */
		//virtual bool		Release( void ) = 0;
		///**
		// *	@return the current reference count for the resource.
		// */
		//virtual UINT32		GetRefCount( void ) const = 0;
	};

} // end namespace GDE

#endif //_RESOURCE_H__Included_355108999

