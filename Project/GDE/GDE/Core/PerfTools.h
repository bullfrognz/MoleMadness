/**
 *	@file GDE\GDE\Core\PerfTools.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _PERFTOOLS_H__Included_374623820
#define _PERFTOOLS_H__Included_374623820

namespace GDE
{

class CLogger;

/**
 *	The performance monitor will record the time elapsed whilst this object is
 *  'alive'.
 */
class CPerformanceMonitor
{
public:
	CPerformanceMonitor( const char* pDescription );
	~CPerformanceMonitor( void );
	static void SetLog( CLogger* pLog );
	static CLogger*	GetLog( void );
protected:
	static CLogger*	sm_pLog;
	const char* m_pDescription;		//!< Description for what happens in this block.
	UINT64		m_uStartTime;		//!< Start time of the block.
	UINT64		m_uFrequency;		//!< Frequency of CPU 
};

} // end namespace GDE

#define _TIME_FUNCTION( func ) \
{\
	GDE::CPerformanceMonitor PerfMon( _TOSTR( func ) );\
	func;\
}

#define _TIME_FUNCTION_AND_MEMORY( func ) \
{\
	VLDResetCounts();	\
	GDE::CPerformanceMonitor PerfMon( _TOSTR( func ) );\
	func;\
	VLDOutputCounts();  \
}

#endif //_PERFTOOLS_H__Included_374623820

