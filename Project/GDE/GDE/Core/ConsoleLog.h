/**
 *	@file GDE\GDE\Core\ConsoleLog.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _CONSOLELOG_H__Included_221140869
#define _CONSOLELOG_H__Included_221140869

#include "Log.h"

namespace GDE
{

/**
 *	A log implementation that writes to the console.
 *	As there is only one console (if any) this is a singleton.
 *
 *	@author Jon Booth.
 */
class CConsoleLog : public ILog
{
public:
	static CConsoleLog& GetInstance( void );

	virtual const CLibraryDescription&	GetDesc( void ) const;
	virtual bool						Write( const char* strMessage );
	virtual bool						Reset( void );
protected:
	CConsoleLog( void )
	{	// empty
	}; 
};

} // end namespace GDE

#endif //_CONSOLELOG_H__Included_221140869

