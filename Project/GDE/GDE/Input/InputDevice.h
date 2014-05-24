/**
 *	@file GDE\GDE\Input\InputDevice.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _INPUTDEVICE_H__Included_717095517
#define _INPUTDEVICE_H__Included_717095517

#include "GDE/Core/Bridge.h"
#include "GDE/Input/InputStates.h"

namespace GDE
{

class IInputDeviceImp;
class IInputChangeSubject;
class CStateChangeSubject;

/**
 *	An input device allows us to set up listeners and a mapping for input. Yay!
 */
class CInputDevice : public TBridge< IInputDeviceImp, CREATE_DLL_ID('I','N','P','T') >
{
public:

	typedef IInputChangeSubject IStateChangeSubject;	//!< This typedef allows old code to use this type, and yet refactors the real implementation externally so it can be forward declared.

	typedef TBridge<IInputDeviceImp,CREATE_DLL_ID('I','N','P','T')> TSuperClass; //!< Convenience typedef to the super class.

	CInputDevice( void );
	~CInputDevice( void );
	bool					LoadDLL( const char* strDLLName );
	void					Update( FLOAT32 fTimeDelta );
	bool					GetInputCode( const char* strName, UINT32& uCode ) const;
	const SInputDesc&		GetInputDesc( UINT32 uCode ) const;

	IStateChangeSubject&	GetDownSubject( void );
	IStateChangeSubject&	GetUpSubject( void );
	IStateChangeSubject&	GetChangeSubject( void );

	const SInputState&		GetState( UINT32 uCode ) const;
	const SInputState&		GetLastState( UINT32 uCode ) const;

	UINT32					GetStateCount( void ) const;

	const static SInputDesc		sm_InvalidDesc;	//!< Returned if a desc of an invalid code is queried.
	const static SInputState	sm_InvalidState;//!< Returned if a state of an invalid code is queried.
protected:
	inline SInputState*			GetCurrentState( void );
	inline SInputState*			GetPreviousState( void );
	inline const SInputState*	GetCurrentState( void ) const;
	inline const SInputState*	GetPreviousState( void ) const;

	CStateChangeSubject*	m_pButtonDownSubject;	//!< An observer subject for button down states.
	CStateChangeSubject*	m_pButtonUpSubject;		//!< An observer subject for button up states.
	CStateChangeSubject*	m_pStateChangedSubject;	//!< An observer subject for any state change.

	UINT32					m_uStateCount;			//!< Number of states the device supports.
	UINT32					m_uCurrentFrame;		//!< The current frame (either zero or one)
	SInputState*			m_pStates;				//!< All the states, this frame and the past one.
	SInputDesc*				m_pDescriptions;		//!< Description of all the states.
//	bool*					m_bPendingWatches;		//!< Watch pending states?
};


}	// end namespace GDE

#endif //_INPUTDEVICE_H__Included_717095517

