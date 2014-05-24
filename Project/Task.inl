//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//
//  File Name :   Task.inl
//
//  Author    :   Bryce Booth
//  Mail      :   bryce.booth@mediadesign.school.nz
//


// Library Includes


// Local Includes


// Implementation


void 
CTask::ResetTimeElapsed()
{
	m_fTimeElapsed = 0.0f;
}


void
CTask::SetPathUpdateTimer(float _fTime)
{
	m_fPathUpdateTimer = _fTime;
}


void
CTask::DecrementSnoozeTime(float _fAmount)
{
	m_fSnoozeTimeLeft -= _fAmount;
}


void
CTask::DecrementStuckTime(float _fTime)
{
	m_fStuckTimeLeft -= _fTime;
}


void
CTask::ClearTarget()
{
	m_iTargetNodeId = -1;
	m_aNodeIdsPath.clear();
	SetPathUpdateTimer(0.0f);
}


void
CTask::SetAcorn(CAcorn* _pAcorn)
{
	m_pAcorn = _pAcorn;
	SetPathUpdateTimer(0.0f);
}


void
CTask::SetTargetNodeId(int _iNodeId)
{
	m_iTargetNodeId = _iNodeId;
	SetPathUpdateTimer(0.0f);
}


void
CTask::SetSnoozeTimeLeft(float _fTime)
{
	m_fSnoozeTimeLeft = _fTime;
}


void
CTask::SetStuckTimeLeft(float _fTime)
{
	m_fStuckTimeLeft = _fTime;
}


CTask::EType
CTask::GetType() const
{
	return (m_eType);
}


CAcorn*
CTask::GetAcorn()
{
	return (m_pAcorn);
}


int
CTask::GetTargetNodeId() const
{
	return (m_iTargetNodeId);
}


float
CTask::GetSnoozeTimeLeft() const
{
	return (m_fSnoozeTimeLeft);
}


float
CTask::GetStuckTimeLeft() const
{
	return (m_fStuckTimeLeft);
}


float
CTask::GetTimeElapsed() const
{
	return (m_fTimeElapsed);
}


float
CTask::GetPathUpdateTimer() const
{
	return (m_fPathUpdateTimer);
}


bool
CTask::HasAcorn() const
{
	return (m_pAcorn != 0);
}


bool
CTask::HasTargetNode() const
{
	return (m_iTargetNodeId != -1);
}