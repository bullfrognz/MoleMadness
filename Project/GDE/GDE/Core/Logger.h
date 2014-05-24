/**
 *	@file GDE\GDE\Core\Logger.h
 *	@brief 
 *	Logging of information for debug purposes.
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _LOGGER_H__Included_947207117
#define _LOGGER_H__Included_947207117

#include "Bridge.h"

namespace GDE
{
	class ILog;

	class CLogger : public TBridge<ILog, CREATE_DLL_ID( 'L','O','G','R' )>
	{
	public:
		typedef TBridge<ILog, CREATE_DLL_ID( 'L','O','G','R' )> TSuperClass; //!< Convenience typedef

		CLogger( ILog* pLog, UINT32 uBufferSize = 1024*2 );
		~CLogger( void );

		int		PrintF( const char* strFormat, ... );
		int		PrintF( UINT32 uLineNum, const char* strFileName, const char* strFormat, ... );
		bool	Reset( void );

		ILog*	GetImplementer( void );
		void	SetImplementer( ILog* pILog );

		// TODO: should these be in a context that can be passed to managers to bridge back?
		static CLogger& GetDebugPrintLog( void );
		static CLogger& GetDebugSPrintLog( void );	
	protected:
		UINT32	m_uBufferSize;
		char*	m_pBuffer;
	};


} // end namespace GDE

#ifdef _DEBUG
/**
 *	The DbgPrint Macro will allow you to send output to the debugger typically.
 */
# define DbgPrint( ... ) GDE::CLogger::GetDebugPrintLog().PrintF( __LINE__, __FILE__, __VA_ARGS__ )
# define DbgSPrint GDE::CLogger::GetDebugSPrintLog().PrintF
#else
# define DbgPrint sizeof // this will do nothing
# define DbgSPrint( ... ) // this will do nothing
#endif
#endif //_LOGGER_H__Included_947207117

