/**
 *	@file GDE\GDE\Input\StateChangeSubject.h
 *	@brief Note this file should only be internal to the input library.
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _STATECHANGESUBJECT_H__Included_1736577653
#define _STATECHANGESUBJECT_H__Included_1736577653

#include "InputChangeSubject.h"

#include <vector> // ah the STL, at last
namespace GDE
{

/**
 *	This internal class is used to send messages.
 */
class CStateChangeSubject : public IInputChangeSubject
{
public:
	virtual bool	AddObserverForAll( IObserver* pObserver );
	virtual bool	RemoveObserverForAll( IObserver* pObserver );
	virtual bool	AddObserver( IObserver* pObserver, UINT32 uMessage );
	virtual bool	RemoveObserver( IObserver* pObserver, UINT32 uMessage );
protected:
	inline bool		FinalAddObserver( IObserver* pObserver, UINT32 uMessage );
	inline bool		FinalRemoveObserver( IObserver* pObserver, UINT32 uMessage );
	inline UINT32	GetObserverPos( IObserver* pObserver, UINT32 uMessage );

	friend CInputDevice;	// allow the input device to call the functions below:
	CStateChangeSubject( CInputDevice& Device );
	bool			StateIsObserved( UINT32 uMessage ) const;
	void			Notify( UINT32 uMessage );
	void			SetNotifying( UINT32 uMessage );

	typedef std::vector< std::vector< IObserver* > > CObserverStore;
	CObserverStore				m_Observers;				//!< The observers per state.
	UINT32						m_uCurrentNotification;		//!< Is this currently notifying?
	std::vector< IObserver* >	m_ObserversToAdd;			//!< Observers to add after this notify.
	std::vector< IObserver* >	m_ObserversToRemove;		//!< Observers to remove after this notify.
};

} // end namespace GDE

#endif //_STATECHANGESUBJECT_H__Included_1736577653

