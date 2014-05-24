/**
 *	@file GDE\GDE\Core\DLLInterface.h
 *	@brief Description of things that are defined on both sides of a dll bridge.
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _DLLINTERFACE_H__Included_2020147865
#define _DLLINTERFACE_H__Included_2020147865

#include "GDE/Core/GDETypes.h"

#ifndef _TOSTR
# define _XSTR( arg ) #arg
# define _TOSTR( arg ) _XSTR( arg )
# define warnmsg(arg) message( __FILE__ "(" _TOSTR(__LINE__) ") : " arg )
#endif

/**
 *	Use this macro to create the library type identifier string.
 *	The argument must be a four character const char*
 */
#define CREATE_DLL_ID( c0, c1, c2, c3 ) (((c0)<<24)+((c1)<<16)+((c2)<<8)+(c3))
/**
 *	@param uDLLId		The id to convert to a string.
 *	@param strBufffer	The string buffer to accept the string - must be at least 5 chars.
 */
inline void DLLIdToString( GDE::UINT32 uDLLId, char* strBuffer )
{
	strBuffer[0] = (uDLLId>>24)&0xFF;
	strBuffer[1] = (uDLLId>>16)&0xFF;
	strBuffer[2] = (uDLLId>>8)&0xFF;
	strBuffer[3] = (uDLLId>>0)&0xFF;
	strBuffer[4] = 0;
}
/**
 *	Use this macro to create the date time string of current compilation.
 */
#define CREATE_COMP_DATE_TIME_STRING() __DATE__##" - "##__TIME__
#define CREATE_LIBRARY_DESCRIPTION( uDLLID0, uDDLLID1, uDDLID2, uDLLID3,																		\
									strDesc,																									\
									uVerMajor, uVerMinor, uVerPoint )																			\
									GDE::CLibraryDescription(																					\
										CREATE_DLL_ID( uDLLID0, uDDLLID1, uDDLID2, uDLLID3 ),													\
										strDesc,																								\
										CREATE_COMP_DATE_TIME_STRING(),																			\
										uVerMajor, uVerMinor, uVerPoint,																		\
										"Library:	   " strDesc "ver (" _TOSTR(uVerMajor) "." _TOSTR(uVerMinor) "." _TOSTR(uVerPoint) ")\n"	\
										"Implements:   " _TOSTR( uDLLID0 )  _TOSTR(uDDLLID1) _TOSTR( uDDLID2 ) _TOSTR( uDLLID3 ) "\n"			\
										"Compiled:	   " CREATE_COMP_DATE_TIME_STRING() "\n" )



namespace GDE
{

	/**
	 *	A version identifier.
	 */
	class CLibraryVersion
	{
	public:
		CLibraryVersion(
			UINT16		uVerMajor,
			UINT8		uVerMinor,
			UINT8		uVerPoint );
	protected:
		UINT8		m_uPointVersion;	//!< Point release number. Typically for bugfixes.
		UINT8		m_uMinorVersion;	//!< Minor version number. Typically for small incremental additional features.
		UINT16		m_uMajorVersion;	//!< Major version number. Typically for refactor and removal of legacy features.
	};
	/**
	 *	The library description defines all the library stuff.
	 */
	class CLibraryDescription
	{
	public:
		/**
		 *	Do not directly call this constructor, instead call the CREATE_LIBRARY_DESCRIPTION() macro above.
		 *	@see CREATE_LIBRARY_DESCRIPTION().
		 */
		CLibraryDescription(
			UINT32		uDLLId,
			const char* strDesc,
			const char* strCompileDateTime,
			UINT16		uVerMajor,
			UINT8		uVerMinor,
			UINT8		uVerPoint,
			const char* strFullDesc );
		const char*	GetDescription( void ) const { return m_strDescription; }
		const char* GetFullDescription( void ) const { return m_strFullDescription; }
		UINT32 GetDLLID( void ) const { return m_uDLL_ID; }
	protected:
		CLibraryDescription& operator=(const CLibraryDescription& Other);	// non implemented assignment, fixes a warning.
		const UINT32			m_uDLL_ID;									//!< The interface ID.
		const char*				m_strDescription;							//!< The description of the library.
		const char*				m_strCompilationDateTime;					//!< String containing the date and time library was compiled. This must be standard form @see CREATE_COMP_DATE_TIME_STRING().
		const CLibraryVersion	m_Version;									//!< The version of the library.
		const char*				m_strFullDescription;						//<! The long description.
	};


	///////////////////////////////////////////////////////////////////////////
	// Inline implementation so only this file (and types.h) need inlcuding.
	inline CLibraryVersion::CLibraryVersion( 
			UINT16		uVerMajor,
			UINT8		uVerMinor,
			UINT8		uVerPoint )
		: m_uPointVersion( uVerPoint )
		, m_uMinorVersion( uVerMinor )
		, m_uMajorVersion( uVerMajor )
	{	// empty
	}

	inline CLibraryDescription::CLibraryDescription(
			UINT32		uDDLId,
			const char* strDesc,
			const char* strCompileDateTime,
			UINT16		uVerMajor,
			UINT8		uVerMinor,
			UINT8		uVerPoint,
			const char* strFullDesc )
		: m_uDLL_ID( uDDLId )
		, m_strDescription( strDesc )
		, m_strCompilationDateTime( strCompileDateTime )
		, m_Version( uVerMajor, uVerMinor, uVerPoint )
		, m_strFullDescription( strFullDesc )
	{	// empty
	}

	const static CLibraryDescription gs_NULLLibraryDesc = CREATE_LIBRARY_DESCRIPTION(
		'N','U','L', 'L',
		"Library not implemented",
		0,0,0);		// version

} // end namespace GDE

#endif //_DLLINTERFACE_H__Included_2020147865

