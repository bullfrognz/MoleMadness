/**
 *	@file GDE\GDE\Core\WindowsAssert.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _WINDOWSASSERT_H__Included_697969209
#define _WINDOWSASSERT_H__Included_697969209

#include "AssertQuery.h"

namespace GDE
{

/**
 *	The windows assert breaks by throwing a message box and asking the user what to do.
 *
 *	@author Jon Booth.
 */
class CWindowsAssertQuery : public IAssertQuery
{
public:
	static CWindowsAssertQuery&			GetInstance( void );
	virtual const CLibraryDescription&	GetDesc( void ) const;
	virtual void						AskAssert( const char* strFileName, UINT32 uLine,const char* strFunction, const char* strMessage );
	virtual bool						ShouldBreak( void ) { return m_bBreak; }
	virtual bool						ShouldAlwaysIgnore( void ) { return m_bAlwaysSkip; }
private:
	CWindowsAssertQuery( void );
	bool m_bBreak;
	bool m_bAlwaysSkip;
};

} // end namespace GDE

#endif //_WINDOWSASSERT_H__Included_697969209

