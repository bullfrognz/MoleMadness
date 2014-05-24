/**
 *	@file GDE\GDE\Input\InputStates.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _INPUTSTATES_H__Included_1004814914
#define _INPUTSTATES_H__Included_1004814914

namespace GDE
{
	/**
	 *	What kind of input state is this?
	 */
	enum EInputStateType
	{
		IST_Button = 0,		//!< The input is a button (0->1)
		IST_Axis,			//!< The input is an axis (-1 -> 1)
		IST_Lever,			//!< The input is a lever (0->1)
		IST_Range,			//!< The input is a float without a range mapped.
		IST_Count			//!< Number of states, also used when invalid.
	};

	/**
	 *	The current value of an input state.
	 */
	struct SInputState
	{
		bool			m_bWatched;		//!< Is this state being observed.
		bool			m_bButtonDown;	//!< Is this button currently down.
		FLOAT32			m_fValue;		//!< What is the axis/lever value for the state?
		FLOAT32			m_fValue2;		//!, What is the second axis/lever value for the state?

		bool			operator!= (const SInputState& Other ) const
		{
			return 
				m_bButtonDown != Other.m_bButtonDown ||
				m_fValue != Other.m_fValue ||
				m_fValue2 != Other.m_fValue2;
		}
	};

	/**
	 *	The description of a given input state.
	 */
	struct SInputDesc
	{
		EInputStateType m_eType;			//!< What type is this input?
		const CHAR8*	m_strName;			//!< Name for this input.
		const CHAR8*	m_strDescription;	//!< The textual description of the input.
		FLOAT32			m_fMin;				//!< The minimum value this state can be.
		FLOAT32			m_fMax;				//!< The maximum value this state can be.
	};

} // end namespace GDE

#endif //_INPUTSTATES_H__Included_1004814914

