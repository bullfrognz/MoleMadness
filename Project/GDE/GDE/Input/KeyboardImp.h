/**
 *	@file GDE\GDE\Input\KeyboardImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _KEYBOARDIMP_H__Included_1748682591
#define _KEYBOARDIMP_H__Included_1748682591

error; // note this does NOT compile as a more generic - input device mapping was created.

#include "GDE/Core/Implementer.h"

namespace GDE
{
	///**
	// *	This enumeration provides an index for each key,
	// *	starting at zero and ending at MAX or KK_Count.
	// */
	//enum EKeyboardKey
	//{ 
	//	KK_Escape = 0,	//!< Escape key.
	//	KK_Backspace,	//!< Backspace key.
	//	KK_Tab,			//!< Tab key.
	//	KK_Enter,		//!< Any of the two enter keys.
	//	KK_Space,		//!< Space key.
	//	KK_Shift,		//!< Any of the two shift keys.
	//	KK_Ctrl,		//!< Any of the two ctrl keys.
	//	KK_Alt,			//!< Any of the two alt keys.
	//	KK_Lwin,		//!< Left windows key.
	//	KK_Rwin,		//!< Right windows key.
	//	KK_Apps,		//!< Applications key.
	//	KK_Pause,		//!< Pause key.
	//	KK_CapsLock,	//!< Caps lock key.
	//	KK_NumLock,		//!< Num lock key.
	//	KK_ScrollLock,	//!< Scroll lock key.
	//	KK_Pgup,		//!< Page up key.
	//	KK_Pgdn,		//!< Page down key.
	//	KK_Home,		//!< Home key.
	//	KK_End,			//!< End key.
	//	KK_Insert,		//!< Insert key.
	//	KK_Delete,		//!< Delete key.
	//	KK_Left,		//!< Left arrow key.
	//	KK_Up,			//!< Up arrow key.
	//	KK_Right,		//!< Right arrow key.
	//	KK_Down,		//!< Down arrow key.
	//	KK_0,			//!< Main keyboard '0' key.
	//	KK_1,			//!< Main keyboard '1' key.
	//	KK_2,			//!< Main keyboard '2' key.
	//	KK_3,			//!< Main keyboard '3' key.
	//	KK_4,			//!< Main keyboard '4' key.
	//	KK_5,			//!< Main keyboard '5' key.
	//	KK_6,			//!< Main keyboard '6' key.
	//	KK_7,			//!< Main keyboard '7' key.
	//	KK_8,			//!< Main keyboard '8' key.
	//	KK_9,			//!< Main keyboard '9' key.
	//	KK_A,			//!< 'A' key.
	//	KK_B,			//!< 'B' key.
	//	KK_C,			//!< 'C' key.
	//	KK_D,			//!< 'D' key.
	//	KK_E,			//!< 'E' key.
	//	KK_F,			//!< 'F' key.
	//	KK_G,			//!< 'G' key.
	//	KK_H,			//!< 'H' key.
	//	KK_I,			//!< 'I' key.
	//	KK_J,			//!< 'J' key.
	//	KK_K,			//!< 'K' key.
	//	KK_L,			//!< 'L' key.
	//	KK_M,			//!< 'M' key.
	//	KK_N,			//!< 'N' key.
	//	KK_O,			//!< 'O' key.
	//	KK_P,			//!< 'P' key.
	//	KK_Q,			//!< 'Q' key.
	//	KK_R,			//!< 'R' key.
	//	KK_S,			//!< 'S' key.
	//	KK_T,			//!< 'T' key.
	//	KK_U,			//!< 'U' key.
	//	KK_V,			//!< 'V' key.
	//	KK_W,			//!< 'W' key.
	//	KK_X,			//!< 'X' key.
	//	KK_Y,			//!< 'Y' key.
	//	KK_Z,			//!< 'Z' key.
	//	KK_Grave,		//!< Grave accent (`).
	//	KK_Minus,		//!< Main keyboard minus key (-).
	//	KK_Equals,		//!< Main keyboard equals key (=).
	//	KK_BackSlash,	//!< Back slash key (\).
	//	KK_LBracket,	//!< Left square bracket ([).
	//	KK_RBracket,	//!< Right square bracket (]).
	//	KK_Semicolon,	//!< Semicolon (;).
	//	KK_Apostrophe,	//!< Apostrophe (').
	//	KK_Comma,		//!< Comma (,).
	//	KK_Period,		//!< Main keyboard period key (.).
	//	KK_Slash,		//!< Main keyboard slash key (/).
	//	KK_NP0,			//!< Numeric keyboard '0' key.
	//	KK_NP1,			//!< Numeric keyboard '1' key.
	//	KK_NP2,			//!< Numeric keyboard '2' key.
	//	KK_NP,			//!< Numeric keyboard '3' key.
	//	KK_NP4,			//!< Numeric keyboard '4' key.
	//	KK_NP5,			//!< Numeric keyboard '5' key.
	//	KK_NP6,			//!< Numeric keyboard '6' key.
	//	KK_NP7,			//!< Numeric keyboard '7' key.
	//	KK_NP8,			//!< Numeric keyboard '8' key.
	//	KK_NP9,			//!< Numeric keyboard '9' key.
	//	KK_NPMultiply,	//!< Numeric keyboard multiply key (*).
	//	KK_NPDivide,	//!< Numeric keyboard divide key (/).
	//	KK_NPAdd,		//!< Numeric keyboard add key (+).
	//	KK_NPSubtract,	//!< Numeric keyboard subtract key (-).
	//	KK_NPDecimal,	//!< Numeric keyboard decimal key (.).
	//	KK_F1,			//!< F1 key.
	//	KK_F2,			//!< F2 key.
	//	KK_F3,			//!< F3 key.
	//	KK_F4,			//!< F4 key.
	//	KK_F5,			//!< F5 key.
	//	KK_F6,			//!< F6 key.
	//	KK_F7,			//!< F7 key.
	//	KK_F8,			//!< F8 key.
	//	KK_F9,			//!< F9 key.
	//	KK_F10,			//!< F10 key.
	//	KK_F11,			//!< F11 key.
	//	KK_F12,			//!< F12 key.
	//	KK_Count,		// Must come last.
	//	MAX = KK_Count	// Used for the observer templates.
	//};

	class IKeyboardImp : public IImplementer
	{
	public:
		virtual void Update( FLOAT32 fTimeDelta );

		virtual UINT32	GetButtonCount( void ) const = 0;
		virtual CHAR8	GetButtonName( UINT32 uKeyCode ) const = 0;
		virtual UINT32	GetButtonCode( CHAR8 strButtonName ) const = 0;
		virtual bool	IsButtonDown( UINT32 uKeyCode ) const = 0;

		//virtual bool AddKeyDownObserver( CObserver* pObserver, EKeyboardKey eKey ) = 0;
		//virtual bool RemoveKeyDownObserver( CObserver* pObserver, EKeyboardKey eKey ) = 0;
		//virtual bool AddKeyDownObserver( CObserver* pObserver ) = 0;
		//virtual bool RemoveKeyDownObserver( CObserver* pObserver eKey ) = 0;

		//virtual bool AddKeyUpObserver( CObserver* pObserver, EKeyboardKey eKey ) = 0;
		//virtual bool RemoveKeyUpDownObserver( CObserver* pObserver, EKeyboardKey eKey ) = 0;
		//virtual bool AddKeyUpObserver( CObserver* pObserver ) = 0;
		//virtual bool RemoveKeyUpDownObserver( CObserver* pObserver ) = 0;
		//virtual bool IsKeyDown( EKeyboardKey eKey ) = 0;


	};
}	// end namespace GDE

#endif //_KEYBOARDIMP_H__Included_1748682591

