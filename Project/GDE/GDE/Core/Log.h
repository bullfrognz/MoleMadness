/**
 *	@file GDE\GDE\Core\Log.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _LOG_H__Included_75218448
#define _LOG_H__Included_75218448

namespace GDE
{

class ILog
{
public:
	virtual ~ILog( void ) {}; // ensure the virtual deconstructor.

	/**
	 *	@returns the unique identifier string for this log implementation.
	 */
	virtual const CLibraryDescription&	GetDesc( void ) const = 0;
	/**
	 *	This function prints out data to the log output.
	 *
	 *	@returns Success.
	 */
	virtual bool		Write( const char* strMessage ) = 0;
	/**
	 *	This function will reset or clear the contents of the log.
	 *
	 *	@returns Success.
	 */
	virtual bool		Reset() = 0;
protected:
};

} // end namespace GDE.

#endif //_LOG_H__Included_75218448

