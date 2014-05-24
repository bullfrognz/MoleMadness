/**
 *	@file GDE\GDE\Core\DebuggerLog.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _DEBUGGERLOG_H__Included_532609268
#define _DEBUGGERLOG_H__Included_532609268

#include "Log.h"

namespace GDE
{

/**
 *	This class defines a singleton that directs log messages to the debugger output window.
 *	As there is only one debugger output window (if any) this is a singleton.
 *
 *	@author Jon Booth.
 */
class CDebuggerLog : public ILog
{
public:
	static CDebuggerLog& GetInstance( void );

	virtual const CLibraryDescription&	GetDesc( void ) const;
	virtual bool						Write( const char* strMessage );
	virtual bool						Reset( void );
protected:
	CDebuggerLog( void )
	{	// empty
	}; 
};

} // end namespace GDE

#endif //_DEBUGGERLOG_H__Included_532609268

