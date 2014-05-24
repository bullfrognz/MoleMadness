/**
 *	@file GDE\GDE\Input\InputDeviceImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _INPUTDEVICEIMP_H__Included_1995006912
#define _INPUTDEVICEIMP_H__Included_1995006912

#include "GDE/Core/Implementer.h"
#include "GDE/Input/InputStates.h"

namespace GDE
{
	/**
	 *	Used to implement an input device.
	 */
	class IInputDeviceImp : public IImplementer
	{
	public:
		/**
		 *	Update the input manager.
		 *	@param fTimeDelta	Time elapsed (in seconds) since last update.
		 */
		virtual void Update( FLOAT32 fTimeDelta ) = 0;

		/**
		 *	@return The number of input states this device supports.
		 */
		virtual UINT32	GetInputCount( void ) const = 0;
		/**
		 *	Fills in all the input state descriptions.
		 *
		 *	@param pDescs	Array to be filled in with descriptions. Must have enough members, @see GetInputCount.
		 */
		virtual void	GetInputDescriptions( SInputDesc* pDescs ) const = 0;
		/**
		 *	Fills in all the current intput state descriptions.
		 *	@see WatchInputState
		 *	@param pDescs	Array to be filled in with descriptions. Must have enough members, @see GetInputCount.
		 *					Only those states marked m_bWatched will be updated.
		 */
		virtual void	GetInputStates( SInputState* pState ) const = 0;
	};
}	// end namespace GDE

#endif //_INPUTDEVICEIMP_H__Included_1995006912

