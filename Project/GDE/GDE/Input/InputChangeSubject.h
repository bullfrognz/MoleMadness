/**
 *	@file GDE\GDE\Input\InputChangeSubject.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _INPUTCHANGESUBJECT_H__Included_809686202
#define _INPUTCHANGESUBJECT_H__Included_809686202

#include "GDE/Core/Observer.h"

namespace GDE
{
class CInputDevice;

/**
 *	This internal class is used to send messages.
 *	Note, no longer internal, so it can be forward declared.
 */
class IInputChangeSubject : public ISubject
{
public:

	virtual bool AddObserverForAll( IObserver* pObserver ) = 0;
	virtual bool RemoveObserverForAll( IObserver* pObserver ) = 0;
	CInputDevice& GetDevice( void ) { return m_Device; }
protected:
	IInputChangeSubject( CInputDevice &Device )
	: m_Device( Device )
	{	// empty
	}
	CInputDevice&	m_Device;
	IInputChangeSubject& operator=(const IInputChangeSubject& Other);	// non implemented assignment, fixes a warning.
};

} // end namespace GDE
#endif //_INPUTCHANGESUBJECT_H__Included_809686202

