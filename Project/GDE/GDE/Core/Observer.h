/**
 *	@file GDE\GDE\Core\Observer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _OBSERVER_H__Included_1658944758
#define _OBSERVER_H__Included_1658944758

namespace GDE
{
	// so this class will help us to build up an observer/subject or
	// listener/subject relationship
	class IObserver;

	class ISubject
	{
	public:
		virtual ~ISubject( void ) {} // ensure all destructors are virtual.
		virtual bool AddObserver( IObserver* pObserver, UINT32 uMessage ) = 0;
		virtual bool RemoveObserver( IObserver* pObserver, UINT32 uMessage ) = 0;
	};


	//class CSubject
	//{
	//public:
	//	bool AddObserver( IObserver* pObserver, UINT32 uMessage );
	//	bool RemoveObserver( IObserver* pObserver, UINT32 uMessage );
	//protected:
	//	CSubject( UINT32 uMaxMessages )
	//		: m_ppObserverListPerMessage( NULL )
	//		, m_uMaxMessages( uMaxMessages )
	//	{
	//		m_ppObserverListPerMessage = new IObserver*[m_uMaxMessages];
	//		for ( UINT32 i=0; i<m_uMaxMessages; i++ )
	//		{
	//			m_ppObserverListPerMessage[ i ] = NULL;
	//		}
	//	}
	//	~CSubject( void )	// protected so subclasses have to create their own public one if required and deal with being virtual as required...
	//	{
	//		// TODO: notify any listeners that the subject is gone?
	//		delete[] m_ppObserverListPerMessage;
	//		m_uMaxMessages = 0;
	//	}
	//	void NotifyObservers( UINT32 uMessage );
	//	IObserver**	m_ppObserverListPerMessage;	//!< Start of a linked list of listeners, one for each message.
	//	UINT32		m_uMaxMessages;				//!< The maximum number of messages this will be able to send.
	//};

	//template <typename TConcreteSubject> class TEnumSubject : public CSubject
	//{
	//public:
	//	typedef typename TConcreteSubject::EObserverMessage EMessage;

	//protected:
	//	/**
	//	 *	If the following doesn't compile it will because the CSubject does 
	//	 *	not contain an EObserverMessage enumeration, or that enumeration
	//	 *	does not contain a MAX value, specifying the maximum value.
	//	 */
	//	TEnumSubject( void )
	//		: CSubject( EMessage::MAX )
	//	{	// empty.
	//	}
	//};

	class IObserver
	{
	public:
		virtual ~IObserver( void ) {};
		/**
		 *	The subject will call this when it is required.
		 *	Our observer/listener has to implement this function
		 */
		virtual void NotifyChange( ISubject* pSubject, UINT32 uMessage ) = 0;
	protected:
	};

	//template <typename TConcreteObserver, typename TConcreteSubject> class TEnumObserver : public IObserver
	//{
	//public:
	//	typedef typename TConcreteSubject::EObserverMessage EMessage;
	//	
	//private:
	//	/**
	//	 *	This implementation simply convenience typecasts for us.
	//	 */
	//	virtual void NotifyChange( CSubject* pSubject, UINT32 eMessage )
	//	{
	//		// If the following doesn't compile, you need to ensure that the child class
	//		// implements the following function.
	//		// if the error is because EMessage is not a type then
	//		// ensure the child class (TConcreteObserver) contains an EMessage enum.
	//		TConcreteObserver::NotifyChange( reinterpret_cast<TConcreteSubject*>(pSubject), EMessage(eMessage) );
	//	}
	//};

}	// end namespace GDE

#endif //_OBSERVER_H__Included_1658944758

