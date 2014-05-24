/**
 *	@file GDE\GDE\Core\Core.h
 *	@brief 
 *	This should always be included first, it is the primary header for the	
 *	GDE (Games Development Engine) library.
 *
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _CORE_H__Included_925621900
#define _CORE_H__Included_925621900

#include "VLD/VLD.h" // note you need to have the VLD/ folder on additional to make this work....

///////////////////////////////////////////////////////////////////////
// Disable exceptions from the standard libraries under visual studio
#define _HAS_EXCEPTIONS 0
#define _STATIC_CPPLIB
///////////////////////////////////////////////////////////////////////

//! this won't compile unless the expression is true.
#define _COMPILE_ASSERT( expr )	typedef char __CASSERT[ (expr) ? 1 : -1 ];
//! returns the number of elements of a static array
#define _ARRAY_SIZE( arg ) ( sizeof( arg )/sizeof( (arg)[0] ) )

//! hide the warning associated with an unused variable (allows searching later)
#define UNUSED_VAR( arg ) (void)arg

// useful to create warnings in visual studio - @pragma warnmsg will give a good warning.
# define _XSTR( arg ) #arg
# define _TOSTR( arg ) _XSTR( arg )
# define warnmsg(arg) message( __FILE__ "(" _TOSTR(__LINE__) ") : " arg )

#ifdef GCC
//! to tell the compiler to generate warnings for inappropriate printf arguments.
# define _PRINTF_FMT( FormatPos, ArgStart ) __attribute__ ((format (printf, FormatPos, ArgStart)))
//! to deprecate a function (should be able to cross compiler setup)?
# define DEPRECATED __attribute__((__deprecated__))
#else
//! to tell the compiler to generate warnings for inappropriate printf arguments.
# define _PRINTF_FMT( FormatPos, ArgStart )
//! to deprecate a function (should be able to cross compiler setup)?
# define DEPRECATED
#endif

#include "GDETypes.h"
#include "Assert.h"



#endif //_CORE_H__Included_925621900

