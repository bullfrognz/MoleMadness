/**
 *	@file GDE\GDE\Core\GDETypes.h
 *	@brief Define the core types to be used throughout the library.
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _GDETYPES_H__Included_51775399
#define _GDETYPES_H__Included_51775399

namespace GDE
{

// don't use standard types directly:
typedef float 			FLOAT32;
typedef double 			FLOAT64;

const static FLOAT32 fEPSILON	= 0.0000000001f; // very small....
const static FLOAT32 fMASSIVE 	= 9999999999999999999999999999999999.f; // very big

typedef unsigned long long int UINT64;
#ifdef UINT64_MAX // for somereason this is defined (but stdint.h not pulled in).
# undef UINT64_MAX
#endif
const UINT64 UINT64_MAX = 0xFFFFFFFFFFFFFFFFULL;

typedef long long signed int SINT64;
const static SINT64 SINT64_MAX = 0x7FFFFFFFFFFFFFFFULL;
const static SINT64 SINT64_MIN = 0x8000000000000000ULL;

typedef unsigned int 	UINT32;
const static UINT32 UINT32_MAX = 0xFFFFFFFF;

typedef signed int		SINT32;
const static SINT32 SINT32_MAX = 0x7FFFFFFF;
const static SINT32 SINT32_MIN = 0x80000000;

typedef unsigned short	UINT16;
const static UINT16 UINT16_MAX	= 0xFFFF;

typedef signed short	SINT16;
const static SINT16 SINT16_MAX	= 0x7FFF;
// enter the number as msvc complains otherwise.
const static SINT16 SINT16_MIN	= -32767; //SINT16(0x8000); // note could be one lower in two's compliment.

typedef unsigned char	UINT8;
const static UINT8 UINT8_MAX 	= 0xFF;

typedef signed char		SINT8;
const static SINT8 SINT8_MAX	= 0x7F;
// enter the number as msvc complains otherwise.
const static SINT8 SINT8_MIN	= -127;// 0x80; // note could be one lower in two's compliment.

typedef char CHAR8;
typedef wchar_t CHAR16;

#ifndef NULL
#define NULL (0)
#endif

} // end namespace GDE

#endif //_GDETYPES_H__Included_51775399

