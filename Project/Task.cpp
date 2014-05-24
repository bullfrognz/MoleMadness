//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  (c) 2011 Bryce Booth
//
//  File Name :   Task.cpp
//
//  Author    :   Bryce Booth
//  Mail      :   bryce.booth@mediadesign.school.nz
//


// Library Includes


// Local Includes


// This Include
#include "Task.h"


// Static Variables
char* CTask::s_cpTaskName[] = {"EXPLORE", "PATROL", "SNOOZE", "HUNT", "STUCK"};


// Implementation


/********************************
            Public
*********************************/





/*---------------------------------------------------------------------------------------------------------------------------
*
* Task constructors
*
*---------------------------------------------------------------------------------------------------------------------------*/

CTask::CTask()
: m_eType(INVALID_TYPE)
, m_pAcorn(0)
, m_iTargetNodeId(-1)
, m_fSnoozeTimeLeft(0)
, m_fTimeElapsed(0)
, m_fPathUpdateTimer(0)
, m_fStuckTimeLeft(0)
{
	//Empty
}





/*---------------------------------------------------------------------------------------------------------------------------
*
* Task deconstructor
*
*---------------------------------------------------------------------------------------------------------------------------*/

CTask::~CTask()
{
	Deinitialise();
}





bool
CTask::Initialise(EType _eType)
{
	m_eType = _eType;


	return (true);
}


void
CTask::Update(float _fDeltaTick)
{
	m_fTimeElapsed += _fDeltaTick;
}


void
CTask::PathPopBack()
{
	m_aNodeIdsPath.pop_back();
	SetPathUpdateTimer(0.0f);
}


void
CTask::SetNodeIdsPath(const std::vector<int>& _karPath)
{
	m_aNodeIdsPath = _karPath;
	SetPathUpdateTimer(0.0f);
}


const std::vector<int>&
CTask::GetNodeIdsPath() const
{
	return (m_aNodeIdsPath);
}





/********************************
            Protected
*********************************/






/********************************
            Private
*********************************/





void
CTask::Deinitialise()
{
	//Empty
}