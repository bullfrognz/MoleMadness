/**
 *	@file GDE\GDE\Core\GameTimer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _GAMETIMER_H__Included_906382882
#define _GAMETIMER_H__Included_906382882

struct timeval;

namespace GDE
{

// note the frequency mightn't be constant!
typedef UINT64 UPerfTime; ///< 64 bit unsigned integer

const UINT32 uNUM_MICROSECONDS = 1000*1000;

// This class should keep time updated etc.
class CSystemTime
{
public:
	bool Update( void );
	void SetTo( UINT32 uSeconds, UINT32 uMicroSeconds );
	void SetTo( FLOAT32 fSeconds );
	UINT64 GetAsUInt( void ) const;
	void SetToUInt( UINT64 uVal );
	CSystemTime operator - ( const CSystemTime& Other ) const;
	const CSystemTime& operator += ( const CSystemTime& Other );
	const CSystemTime& operator %= ( const CSystemTime& Other );
	
	bool operator <= ( const CSystemTime& Other ) const;
	bool operator == ( const CSystemTime& Other ) const;

	UINT32 GetMicroSeconds( void ) const;

	FLOAT32  GetSeconds( void ) const;

	FLOAT32 GetSecondRatio( void );

	static void Sleep( const CSystemTime& Delay );
	static void Sleep( FLOAT32 fSeconds );

	static UINT32 GetMilliTicks( void );
	static UPerfTime GetPerfTimer( void );

	// how many of them per second - ?
	static UINT32 GetPerfTimerFreq( UINT32 uTime = 10 );
	void SetToDiff(
		UPerfTime uStart,
		UPerfTime uEnd,
		UINT32 uFreq);

protected:
	void FillTimeVal( timeval& t ) const;

	UINT32 	m_uSeconds;
	UINT32 	m_uMicroSeconds;
};

class CClock
{
public:
	CClock( void );

	void FrameUpdate( void );
	void FakeFrameInterval( FLOAT32 fFrameInterval );
	FLOAT32 GetFrameInterval( void ) const;
	UINT32 GetFrameNum( void ) const;
	FLOAT32 GetSpeedOfTime( void ) const;
	void SetSpeedOfTime( FLOAT32 fV );
	FLOAT32 GetCyclicalOverTime( FLOAT32 fSeconds );
	void SetMinFrameInterval( FLOAT32 fV );
protected:
	UINT32 		m_uFrameNum;
	FLOAT32		m_fFrameInterval;
	FLOAT32		m_fSpeedOfTime;		//!< ratio of real time to measured time
	CSystemTime	m_LastTime;
	FLOAT32		m_fMinFrameInterval;	//!< Minimum frame interval;
};

} // end namespace GDE

#endif //_GAMETIMER_H__Included_906382882

