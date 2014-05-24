/**
 *	@file GDE\GDE\Core\AssertQuery.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ASSERTQUERY_H__Included_668769834
#define _ASSERTQUERY_H__Included_668769834

namespace GDE
{

/**
 *	When an assert fires, it will use an Implementer of this class to complete the assert query.
 */
class IAssertQuery
{
public:
	virtual const CLibraryDescription&	GetDesc( void ) const = 0;
	virtual void						AskAssert(  const char* strFileName, UINT32 uLine,const char* strFunction, const char* strMessage ) = 0;
	virtual bool						ShouldBreak( void ) = 0;
	virtual bool						ShouldAlwaysIgnore( void ) = 0;
};

} // end namespace GDE

#endif //_ASSERTQUERY_H__Included_668769834

