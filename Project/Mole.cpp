//
//  Media Design School
//  Auckland
//  New Zealand
//
//  File Name :   Mole.cpp
//
//  Copyright © 2004-2010 Media Design School. All Rights Reserved.
//


// Local Includes
#include "MMMContext.h"
#include "Common.h"
#include "Squirrel.h"
#include "Mole.h"
#include "Wall.h"
#include "Circle.h"
#include "Box2D.h"
#include "Acorn.h"



// Library Includes
#include "GDE/Core/Rect.h"
#include "GDE/Renderer/Sprite.h"
#include "GDE/Renderer/SpriteManager.h"
#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"
#include "GDE/Renderer/Vertex.h"
#include "GDE/Renderer/FontManager.h"
#include "GDE/Renderer/Font.h"


#include <math.h>
#include <cassert>


// This Include
#include "Mole.h"


// Static Variables
int CMole::s_iReffCount = 0;
CPath* CMole::s_pSharedPath = 0;
std::map<CAcorn*, bool> CMole::s_mKnownAcorns;


// Implementation


/********************************
            Public
*********************************/





/*---------------------------------------------------------------------------------------------------------------------------
*
* Mole constructors
*
*---------------------------------------------------------------------------------------------------------------------------*/

CMole::CMole()
: m_pSprite(0)
, m_pWhiskers(0)
, m_fHeading(0)
, m_fTranslationTimer(0)
, m_fRadius(fMOLE_RADIUS)
, m_fMovementVelocity(MOVEMENT_VELOCITY)
, m_fRotationVelocity(fDegsToRad(ROTATION_VELOCITY))
, m_iDifficultyLevel(0)
, m_bHighlighted(false)
, m_bHasPatrolTask(false)
{
	if (s_iReffCount == 0)
	{
		const int kiMapWidth = CMMMContext::GetInstance().GetDisplayWidth();
		const int kiMapHeight = CMMMContext::GetInstance().GetDisplayHeight();


		s_pSharedPath = new CPath();
		s_pSharedPath->Initialise(static_cast<float>(kiMapWidth), static_cast<float>(kiMapHeight), 32.0f);
	}


	++ s_iReffCount;
}





/*---------------------------------------------------------------------------------------------------------------------------
*
* Mole deconstructor
*
*---------------------------------------------------------------------------------------------------------------------------*/

CMole::~CMole()
{
	Deinitialise();


	-- s_iReffCount;


	if (s_iReffCount == 0)
	{

		delete s_pSharedPath;
		s_pSharedPath = 0;
#ifndef _EDITOR
		s_mKnownAcorns.clear();
#endif
	}
}



CPath*
CMole::GetPath()
{
	return (s_pSharedPath);
}





/*---------------------------------------------------------------------------------------------------------------------------
*
*	Initialise the Mole at a given position and texture name, returning succes.
*
*---------------------------------------------------------------------------------------------------------------------------*/

bool
CMole::Initialise(const CFVec2& _kvrPosition, const char* _kcpSpriteTexture, float _fHeading)
{
	m_vPosition = _kvrPosition;
	m_vLastPosition = _kvrPosition;
	m_fHeading = _fHeading;


	FW_VALIDATE(InitialiseSprite(_kcpSpriteTexture));

#ifndef _EDITOR
	
	FW_VALIDATE(IntiailiseWhiskers());
	AppendExploreTask();

#endif


	return (true);
}





/*---------------------------------------------------------------------------------------------------------------------------
*
*	Updates the mole for this frame.
*
*---------------------------------------------------------------------------------------------------------------------------*/
#ifndef _EDITOR

void
CMole::Update(float _fTimeDelta, CSquirrel& _rSquirrel)
{
	// Check interection with squirrel
	CCircle Bounds;
	Bounds.Initialise( m_vPosition, m_fRadius );


	if (_rSquirrel.Intersects(Bounds))
	{
		_rSquirrel.LoseLife();
	}


	if (!m_Tasks.empty())
	{
		ProcessStuckManager(_fTimeDelta);


		CTask& rCurrentTask = m_Tasks.top();


		// Check if mole is not patrolling
		if (!m_bHasPatrolTask &&
			rCurrentTask.GetType() != CTask::TYPE_PATROL ||
			rCurrentTask.GetType() != CTask::TYPE_SNOOZE)
		{
			// Try and bags an acorn to patrol, if any
			BagsAcorn();
		}


		switch (rCurrentTask.GetType())
		{
		case CTask::TYPE_EXPLORE:
			ProcessExplore(_fTimeDelta, rCurrentTask);
			break;

		case CTask::TYPE_PATROL:
			ProcessPatrol(_fTimeDelta, rCurrentTask);
			break;

		case CTask::TYPE_SNOOZE:
			ProcessSnooze(_fTimeDelta, rCurrentTask);
			break;

		case CTask::TYPE_HUNT:
			ProcessHunt(_fTimeDelta, rCurrentTask);
			break;

		case CTask::TYPE_STUCK:
			ProcessStuck(_fTimeDelta, rCurrentTask);
			break;
		}
	}
	else
	{
		assert_now("Mole has no assignments");
	}


	s_pSharedPath->NotifyAccessablePosition(m_vPosition, m_fRadius);
	UpdateWhiskers();
	m_Repulsor.Initialise(m_vPosition, fMOLE_RADIUS, 40.0f);
}

#endif





/*---------------------------------------------------------------------------------------------------------------------------
*
*	Renders the mole and debug information.
*
*---------------------------------------------------------------------------------------------------------------------------*/

void
CMole::Draw(bool _bDrawKnownAcorns)
{
	_bDrawKnownAcorns;
	m_pSprite->RenderEx( m_vPosition.X(), m_vPosition.Y(), m_fHeading, 1.0f, 1.0f );


	//if ( m_bHighlighted )
	{
		CCircle Bounds;
		Bounds.Initialise( m_vPosition, m_fRadius );
		CMMMContext::GetInstance().DrawCircle( Bounds, 0xFFFF0000 );
	}


#ifndef _EDITOR

	DrawWhiskers();
	DrawTaskPathTarget(m_Tasks.top());


	if (m_Tasks.top().GetType() == CTask::TYPE_PATROL)
	{
		DrawTargetAcorn(m_Tasks.top().GetAcorn());
	}


	DrawTaskType();
	m_Repulsor.Draw();
	DrawSteeringForce();

	if (_bDrawKnownAcorns)
	{
		DrawKnownAcorns();
	}

#endif
}





/*---------------------------------------------------------------------------------------------------------------------------
*
*	Tells the mole about a near by disturbance
*
*---------------------------------------------------------------------------------------------------------------------------*/

void
CMole::NotifyDisturbance(const CCircle& _krCircle)
{
	_krCircle.Radius();
#ifndef _EDITOR
	// Check disterbance within range
	if (sqrt(_krCircle.Centre().SquareDistance(m_vPosition)) - _krCircle.Radius() - m_fRadius <= 0.0f)
	{
		AppendHuntTask(_krCircle.Centre());
	}
#endif
}





void
CMole::NotifyAcorn(CAcorn* _pAcorn)
{
	_pAcorn;

#ifndef _EDITOR
	s_mKnownAcorns.insert( std::pair<CAcorn*, bool>(_pAcorn, false) );
	BagsAcorn();
#endif
}





void
CMole::NotifyAcornDropped(CAcorn* _pAcorn)
{
	_pAcorn;

#ifndef _EDITOR

	std::map<CAcorn*, bool>::iterator Current = s_mKnownAcorns.begin();
	std::map<CAcorn*, bool>::iterator End = s_mKnownAcorns.end();


	for (Current; Current != End; ++ Current)
	{
		if ((*Current).first == _pAcorn)
		{
			_pAcorn->SetMole(0);
			(*Current).second = false;
			break;
		}
	}

#endif
}





/*---------------------------------------------------------------------------------------------------------------------------
*
*	Tests for interection with circle.
*
*---------------------------------------------------------------------------------------------------------------------------*/

bool
CMole::Intersects(const CCircle& _krCircle) const
{
	CCircle Bounds;
	Bounds.Initialise(m_vPosition, m_fRadius);


	return (_krCircle.Overlaps(Bounds));
}





/*---------------------------------------------------------------------------------------------------------------------------
*
*	Tests for interection with box.
*
*---------------------------------------------------------------------------------------------------------------------------*/

bool
CMole::Intersects(const CBox2D& _krBox) const
{
	CCircle Bounds;
	Bounds.Initialise(m_vPosition, m_fRadius);


	return (_krBox.Intersects(Bounds));
}




void
CMole::UpdateDifficulty(int _iLevelId)
{
	m_iDifficultyLevel = _iLevelId;
}





/********************************
            Protected
*********************************/



#ifndef _EDITOR


// Find a free acorn to start patrolling
void
CMole::BagsAcorn()
{
	if (!m_bHasPatrolTask)
	{
		std::map<CAcorn*, bool>::iterator Current = s_mKnownAcorns.begin();
		std::map<CAcorn*, bool>::iterator End = s_mKnownAcorns.end();


		for (Current; Current != End; ++ Current)
		{
			// Check not bags yet
			if ((*Current).second == false)
			{
				// Check acorn still aviable
				if (((*Current).first)->GetState() == CAcorn::ES_Available)
				{
					AppendPatrolTask((*Current).first);


					(*Current).first->SetMole(this);
					(*Current).second = true;
					break;
				}
				else
				{
					// Do nothing
				}
			}
		}
	}
	else
	{
		// Do nothing
	}
}


void
CMole::AppendExploreTask()
{
	CTask tExplore;
	tExplore.Initialise(CTask::TYPE_EXPLORE);


	m_Tasks.push(tExplore);
}


void
CMole::AppendPatrolTask(CAcorn* _pAcorn)
{
	CTask tPatrol;
	tPatrol.Initialise(CTask::TYPE_PATROL);
	tPatrol.SetAcorn(_pAcorn);


	m_Tasks.push(tPatrol);


	m_bHasPatrolTask = true;



}


void
CMole::AppendSnoozeTask(float _fTime)
{
	CTask tSleep;
	tSleep.Initialise(CTask::TYPE_SNOOZE);
	tSleep.SetSnoozeTimeLeft(_fTime);


	m_Tasks.push(tSleep);
}


void
CMole::AppendHuntTask(const CFVec2& _kvrTargetPosition)
{
	// Remove all hunt tasks before this
	while (m_Tasks.top().GetType() == CTask::TYPE_HUNT)
	{
		m_Tasks.pop();
	}



	CTask tHunt;
	tHunt.Initialise(CTask::TYPE_HUNT);


	int iTargetNode = s_pSharedPath->GenerateNodeId(s_pSharedPath->GetNode(_kvrTargetPosition));


	std::vector<int> aNodeIdsPath;
	bool bPathFound = s_pSharedPath->FindPath(m_vPosition, iTargetNode, aNodeIdsPath);


	if (bPathFound)
	{
		tHunt.SetTargetNodeId(iTargetNode);
		tHunt.SetNodeIdsPath(aNodeIdsPath);
		m_Tasks.push(tHunt);
	}
	else
	{
		// Cannot reach target
		assert(false);
	}
}


void
CMole::AppendStuckTask(float _fTime)
{
	CTask tStuck;
	tStuck.Initialise(CTask::TYPE_STUCK);
	tStuck.SetStuckTimeLeft(_fTime);


	m_Tasks.push(tStuck);
}


void
CMole::ProcessStuckManager(float _fTimeDelta)
{
	if (m_Tasks.top().GetType() != CTask::TYPE_STUCK &&
		m_Tasks.top().GetType() != CTask::TYPE_SNOOZE)
	{
		m_fTranslationTimer += _fTimeDelta;
		m_vTranslationTracker += m_vLastPosition - m_vPosition;


		if (m_fTranslationTimer > 4.0f)
		{
			if (m_vTranslationTracker.Magnitude() < 10.0f)
			{
				AppendStuckTask(2.0f);
			}
			else
			{
				// Do nothing
			}


			m_vTranslationTracker.X(0);
			m_vTranslationTracker.Y(0);
			m_fTranslationTimer = 0.0f;
		}


		m_vLastPosition = m_vPosition;
	}
}


void
CMole::ProcessExplore(float _fTimeDelta, CTask& _rTask)
{
	UpdateTaskTargetPath(_fTimeDelta, _rTask);


	if (!_rTask.HasTargetNode() ||
		(s_pSharedPath->GetNode(_rTask.GetTargetNodeId()).bDiscovered && s_pSharedPath->HasUndiscoveredNodesLeft()))
	{
		int iTargetNodeId = 0;
		
		
		bool bFoundNode = s_pSharedPath->FindUndiscoveredNode(m_vPosition, m_fRadius, m_fHeading, iTargetNodeId);


		if (!bFoundNode)
		{
			s_pSharedPath->FindRandomAccesableNode(iTargetNodeId);
			bFoundNode = true;
		}


		if (bFoundNode)
		{
			_rTask.SetTargetNodeId(iTargetNodeId);
			UpdateTaskTargetPath(_fTimeDelta, _rTask);
		}
	}


	// If have target
	if (_rTask.HasTargetNode())
	{
		CFVec2 vSteerForce;
		ComputeSteerForce(_rTask.GetNodeIdsPath().back(), vSteerForce);


		UpdateRotation(_fTimeDelta, vSteerForce);
		UpdatePosition(_fTimeDelta);
	}
}


void
CMole::ProcessPatrol(float _fTimeDelta, CTask& _rTask)
{
	// Check acorn is no longer avaiable
	if (_rTask.GetAcorn()->GetState() != CAcorn::ES_Available || 
		s_mKnownAcorns[_rTask.GetAcorn()] == false ||
		_rTask.GetAcorn()->GetMole() != this)
	{
		m_Tasks.pop();
		m_bHasPatrolTask = false;
	}
	else
	{
		UpdateTaskTargetPath(_fTimeDelta, _rTask);
		

		// Check have no target
		if (_rTask.GetTargetNodeId() == -1)
		{
			int iRandomTargetId = 0;


			// Find new target
			bool bRandomNodeFound = s_pSharedPath->FindRandomAccesableNode(_rTask.GetAcorn()->GetPosition(), PARTROL_RADIUS, iRandomTargetId);


			if (bRandomNodeFound)
			{
				std::vector<int> aNodeIdsPath;
				bool bPathFound = s_pSharedPath->FindPath(m_vPosition, iRandomTargetId, aNodeIdsPath);

			
				if (bPathFound)
				{
					_rTask.SetTargetNodeId(iRandomTargetId);
					_rTask.SetNodeIdsPath(aNodeIdsPath);
				}
				else
				{
					// Do nothing
				}
			}
			else
			{
				// Do nothing
			}
		}


		// Check has target
		if (_rTask.GetTargetNodeId() != -1)
		{
			
			CFVec2 vSteerForce;
			ComputeSteerForce(_rTask.GetNodeIdsPath().back(), vSteerForce);


			UpdateRotation(_fTimeDelta, vSteerForce);
			UpdatePosition(_fTimeDelta);


			// Check within in range of target to be able to add onto sleep timer
			if (sqrt(_rTask.GetAcorn()->GetPosition().SquareDistance(m_vPosition)) - PARTROL_RADIUS - (PARTROL_RADIUS / 4) <= 0.0f)
			{
				_rTask.Update(_fTimeDelta);


				// Check if have been patroling for 4 seconds
				if (_rTask.GetTimeElapsed() > 4.0f)
				{
					float fSleepTime = static_cast<float>(SLEEP_TIME) - (DIFFICULTY_SLEEP_TIME_DESC * m_iDifficultyLevel);


					// Sleep for 5 seconds
					AppendSnoozeTask(fSleepTime);
					_rTask.ResetTimeElapsed();
				}
				else
				{
					// Do nothing
				}
			}
		}
		else
		{
			// Do nothing
		}
	}
}


void
CMole::ProcessSnooze(float _fTimeDelta, CTask& _rTask)
{
	_rTask.DecrementSnoozeTime(_fTimeDelta);


	// Check sleep is over
	if (_rTask.GetSnoozeTimeLeft() < 0.0f)
	{
		m_Tasks.pop();	
	}
}


void
CMole::ProcessHunt(float _fTimeDelta, CTask& _rTask)
{
	UpdateTaskTargetPath(_fTimeDelta, _rTask);


	// If have target
	if (_rTask.HasTargetNode())
	{
		CFVec2 vSteerForce;
		ComputeSteerForce(_rTask.GetNodeIdsPath().back(), vSteerForce);


		UpdateRotation(_fTimeDelta, vSteerForce);
		UpdatePosition(_fTimeDelta);
	}
	else
	{
		// Reached disternace position
		m_Tasks.pop();
	}
}


void
CMole::ProcessStuck(float _fTimeDelta, CTask& _rTask)
{
	_rTask.DecrementStuckTime(_fTimeDelta);


	if (_rTask.GetStuckTimeLeft() <= 0.0f)
	{
		m_Tasks.pop();
	}


	CFVec2 vOpositeLook(0.0f, -1.0f);
	vOpositeLook.RotateZ(m_fHeading);
	vOpositeLook.RotateZ(fDegsToRad(180));


	UpdateRotation(_fTimeDelta, vOpositeLook);
	UpdatePosition(_fTimeDelta);
}


void
CMole::UpdateTaskTargetPath(float _fTimeDelta, CTask& _rTask)
{
	// Has target
	if (_rTask.HasTargetNode())
	{
		const int kiTargetNodeId = _rTask.GetTargetNodeId();


		// Reached target
		if (s_pSharedPath->IsIntersectingNode(m_vPosition, m_fRadius, kiTargetNodeId))
		{
			_rTask.ClearTarget();
		}
		else
		{
			// Check path update needed
			if (_rTask.GetPathUpdateTimer() <= 0.0f)
			{
				// Refresh path
				std::vector<int> aNewPath;
				bool bPathFound = s_pSharedPath->FindPath(m_vPosition, kiTargetNodeId, aNewPath);


				if (bPathFound)
				{
					_rTask.SetNodeIdsPath(aNewPath);
				}


				_rTask.SetPathUpdateTimer(0.1f);
			}
			else
			{
				// Update timer
				_rTask.SetPathUpdateTimer(_rTask.GetPathUpdateTimer() - _fTimeDelta);
			}
		}
	}
	else
	{
		// Do nothing
	}
}


void
CMole::UpdateRotation(float _fTimeDelta, const CFVec2& _kvrForce)
{
	if (_kvrForce.X() != 0.0f ||
		_kvrForce.Y() != 0.0f)
	{
		// Create heading vector
		CFVec2 vHeading(0.0f, -1.0f);
		vHeading.RotateZ(m_fHeading);


		// Cross
		float fCross = 0.0f;
		fCross  = vHeading.X() * _kvrForce.Y();
		fCross -= _kvrForce.X() * vHeading.Y();


		float fRotationVelocity = m_fRotationVelocity + fDegsToRad(static_cast<float>(DIFFICULTY_ROTATION_VELOCITY_INCR * m_iDifficultyLevel));


		if (fCross > 0.0f)
		{
			// Turn left
			m_fHeading += fRotationVelocity * _fTimeDelta;
		}
		else
		{
			// Turn right
			m_fHeading += fRotationVelocity * - _fTimeDelta;
		}
	}
}


void
CMole::UpdatePosition(float _fTimeDelta)
{
	CCircle SelfCircle;
	CWall* pWalls = 0;
	CAcorn* pAcorns = 0;
	CMole* pMoles = 0;
	int iNumWalls = 0;
	int iNumAcorns = 0;
	int iNumMoles = 0;
	bool bCollision = false;


	// Create heading vector
	CFVec2 vHeading(0.0f, -1.0f);
	vHeading.RotateZ(m_fHeading);


	float fMovementVelocity = m_fMovementVelocity + (DIFFICULTY_MOVEMENT_VELOCITY_INCR * m_iDifficultyLevel);


	CFVec2 vNextPosition = m_vPosition;
	vNextPosition += vHeading * (fMovementVelocity * _fTimeDelta);


	SelfCircle.Initialise(vNextPosition, m_fRadius);


	CMMMContext& rMMMContext = CMMMContext::GetInstance();
	rMMMContext.GetWalls(pWalls, iNumWalls);
	rMMMContext.GetAcorns(pAcorns, iNumAcorns);
	rMMMContext.GetMoles(pMoles, iNumMoles);

	

	CCircle EntityCircle;
	SelfCircle.Initialise(vNextPosition, m_fRadius);


	// Check collision against other moles
	for (int i = 0; i < iNumMoles; ++ i)
	{
		if (&pMoles[i] != this)
		{
			EntityCircle.Initialise(pMoles[i].GetPosition(), pMoles[i].GetRadius());


			if (SelfCircle.Overlaps(EntityCircle))
			{
				bCollision = true;
				break;
			}
			else
			{
				// Do nothing
			}
		}
	}


	if (!bCollision)
	{
		// Check collision against walls
		for (int i = 0; i < iNumWalls; ++ i)
		{
			if (SelfCircle.OverlapsLineSegment(pWalls[i].GetEnd0(), pWalls[i].GetEnd1()))
			{
				bCollision = true;


				break;
			}
		}
	}


	if (!bCollision)
	{
		m_vPosition = vNextPosition;
	}
}


void
CMole::UpdateWhiskers()
{
	for (int i = 0; i < MAX_WHISKER; ++ i)
	{
		m_pWhiskers[i].Update(m_vPosition, m_fHeading);
	}
}


void
CMole::DrawWhiskers() const
{
	for (int i = 0; i < MAX_WHISKER; ++ i)
	{
		m_pWhiskers[i].Draw();
	}
}





void
CMole::DrawTaskPathTarget(CTask& _rTask) const
{
	int iTargetNodeId = _rTask.GetTargetNodeId();
	const std::vector<int>& karNodeIdsPath = _rTask.GetNodeIdsPath();
	GDE::CRenderer* pRenderer = CMMMContext::GetInstance().GetRenderer();


	if (karNodeIdsPath.size() > 0)
	{
		// Draw line to first target
		const CFVec2& kvrNextPosition = s_pSharedPath->GetNode(karNodeIdsPath.back()).vPosition;


		GDE::SVertex FirstLine[2];
		FirstLine[0].m_uColour = 0xFF509999;
		FirstLine[1].m_uColour = 0xFF509999;


		FirstLine[0].m_fX = m_vPosition.X();
		FirstLine[0].m_fY = m_vPosition.Y();
		FirstLine[0].m_fZ = 0.0f;

		FirstLine[1].m_fX = kvrNextPosition.X();
		FirstLine[1].m_fY = kvrNextPosition.Y();
		FirstLine[1].m_fZ = 0.0f;


		pRenderer->DrawPrimitives(GDE::PT_LineList, 1, FirstLine);


		// Draw path lines
		for (int i = karNodeIdsPath.size() - 1; i > 0; -- i)
		{
			const CFVec2& kvrCurrentPosition = s_pSharedPath->GetNode(karNodeIdsPath[i]).vPosition;
			const CFVec2& kvrNextPosition = s_pSharedPath->GetNode(karNodeIdsPath[i - 1]).vPosition;


			GDE::SVertex Vertex[2];
			Vertex[0].m_uColour = 0xFF509999;
			Vertex[1].m_uColour = 0xFF509999;


			Vertex[0].m_fX = kvrCurrentPosition.X();
			Vertex[0].m_fY = kvrCurrentPosition.Y();
			Vertex[0].m_fZ = 0.0f;

			Vertex[1].m_fX = kvrNextPosition.X();
			Vertex[1].m_fY = kvrNextPosition.Y();
			Vertex[1].m_fZ = 0.0f;


			pRenderer->DrawPrimitives(GDE::PT_LineList, 1, Vertex);
		}
	}


	if (iTargetNodeId != -1)
	{
		// Draw cross on target
		const Path::TNode& ktrTargetNode = s_pSharedPath->GetNode(iTargetNodeId);


		GDE::SVertex Cross[4];
		Cross[0].m_uColour = 0xFF509999;
		Cross[1].m_uColour = 0xFF509999;
		Cross[2].m_uColour = 0xFF509999;
		Cross[3].m_uColour = 0xFF509999;


		// One
		Cross[0].m_fX = ktrTargetNode.vPosition.X() - 10;
		Cross[0].m_fY = ktrTargetNode.vPosition.Y() - 10;
		Cross[0].m_fZ = 0.0f;

		Cross[1].m_fX = ktrTargetNode.vPosition.X() + 10;
		Cross[1].m_fY = ktrTargetNode.vPosition.Y() + 10;
		Cross[1].m_fZ = 0.0f;


		// Two
		Cross[2].m_fX = ktrTargetNode.vPosition.X() - 10;
		Cross[2].m_fY = ktrTargetNode.vPosition.Y() + 10;
		Cross[2].m_fZ = 0.0f;

		Cross[3].m_fX = ktrTargetNode.vPosition.X() + 10;
		Cross[3].m_fY = ktrTargetNode.vPosition.Y() - 10;
		Cross[3].m_fZ = 0.0f;


		pRenderer->DrawPrimitives(GDE::PT_LineList, 2, Cross);
	}
}





void
CMole::DrawTargetAcorn(const CAcorn* _kpAcorn) const
{
	GDE::CRenderer* pRenderer = CMMMContext::GetInstance().GetRenderer();


	GDE::SVertex FirstLine[2];
	FirstLine[0].m_uColour = 0xFFFFFF00;
	FirstLine[1].m_uColour = 0xFFFFFF00;


	FirstLine[0].m_fX = m_vPosition.X();
	FirstLine[0].m_fY = m_vPosition.Y();
	FirstLine[0].m_fZ = 0.0f;

	FirstLine[1].m_fX = _kpAcorn->GetPosition().X();
	FirstLine[1].m_fY = _kpAcorn->GetPosition().Y();
	FirstLine[1].m_fZ = 0.0f;


	pRenderer->DrawPrimitives(GDE::PT_LineList, 1, FirstLine);

	CCircle AcornCircle;
	AcornCircle.Initialise(_kpAcorn->GetPosition(), 14.0f);


	AcornCircle.Draw(0xAAFFFF00, true);
}





void
CMole::DrawTaskType() const
{
	GDE::CFont* pFont = CMMMContext::GetInstance().GetFont();


	pFont->Render(m_vPosition.X(), m_vPosition.Y() + 20, GDE::FJ_Centre, CTask::s_cpTaskName[m_Tasks.top().GetType()]); 
}




void
CMole::DrawSteeringForce() const
{
	// Draw line to first target
	GDE::CRenderer* pRenderer = CMMMContext::GetInstance().GetRenderer();


	GDE::SVertex SteerForceLine[2];
	SteerForceLine[0].m_uColour = 0xFF00FF00;
	SteerForceLine[1].m_uColour = 0xFF00FF00;


	SteerForceLine[0].m_fX = m_vPosition.X();
	SteerForceLine[0].m_fY = m_vPosition.Y();
	SteerForceLine[0].m_fZ = 0.0f;

	SteerForceLine[1].m_fX = m_vPosition.X() + (m_vLastSteeringForce.X() * 20.0f);
	SteerForceLine[1].m_fY = m_vPosition.Y() + (m_vLastSteeringForce.Y() * 20.0f);
	SteerForceLine[1].m_fZ = 0.0f;


	pRenderer->DrawPrimitives(GDE::PT_LineList, 1, SteerForceLine);
}




void
CMole::DrawKnownAcorns() const
{
	std::map<CAcorn*, bool>::iterator Current = s_mKnownAcorns.begin();
	std::map<CAcorn*, bool>::iterator End = s_mKnownAcorns.end();


	for (Current; Current != End; ++ Current)
	{
		// Draw known unbags acorns
		if ((*Current).second == false)
		{
			CCircle AcornCircle;
			AcornCircle.Initialise((*Current).first->GetPosition(), 14.0f);
			AcornCircle.Draw(0xAAFA00FF, true);
		}
	}
}





void
CMole::ComputeSteerForce(int _iTargetNodeId, CFVec2& _vrSteerForce)
{
	CFVec2 vSeekForce;
	CFVec2 vWallForce;
	CFVec2 vRepulseForce;

	
	if (_iTargetNodeId != -1)
	{
		ComputeSeekForce(vSeekForce, _iTargetNodeId);
	}


	ComputeRepulsorForce(vRepulseForce);
	ComputeWallForce(vWallForce);


	_vrSteerForce  = vSeekForce;
	_vrSteerForce += vRepulseForce;
	_vrSteerForce += vWallForce;


	m_vLastSteeringForce = _vrSteerForce;
}





void
CMole::ComputeSeekForce(CFVec2& _vrSeekForce, int _iTargetNodeId)
{
	const Path::TNode& ktrTargetNode = s_pSharedPath->GetNode(_iTargetNodeId);


	CFVec2 vSeekForce;
	vSeekForce  = ktrTargetNode.vPosition;
	vSeekForce -= m_vPosition;
	vSeekForce.Normalise();


	_vrSeekForce = vSeekForce;
}





void
CMole::ComputeRepulsorForce(CFVec2& _vrRepelForce)
{
	// Get wall repulsors
	CWall* pWalls = 0;
	int iNumWalls = 0;	


	// Get mole repulsors
	CMole* pMoles = 0;
	int iNumMoles = 0;


	CMMMContext& rMMMContext = CMMMContext::GetInstance();
	rMMMContext.GetWalls(pWalls, iNumWalls);
	rMMMContext.GetMoles(pMoles, iNumMoles);


	// Get repulsors
	std::vector<CRepulsor*> m_aRepulsors;


	for (int i = 0; i < iNumWalls; ++ i)
	{
		m_aRepulsors.push_back( &pWalls[i].GetRepulsor(0) );
		m_aRepulsors.push_back( &pWalls[i].GetRepulsor(1) );
	}


	for (int i = 0; i < iNumMoles; ++ i)
	{
		if (&pMoles[i] != this)
		m_aRepulsors.push_back( pMoles[i].GetRepulsor() );
	}



	for (unsigned int j = 0; j < m_aRepulsors.size(); ++ j)
	{
		CFVec2 vRepulseForce(0.0f, 0.0f);


		CFVec2 vRepulsorPosition = m_aRepulsors[j]->GetPosition();
		float fRepulsorRadius    = m_aRepulsors[j]->GetRadius();
		float fRepulsionRadius   = m_aRepulsors[j]->GetRepulsionRadius();

			
		// Displacement to repulsor
		CFVec2 vDisplacement(m_vPosition);
		vDisplacement -= vRepulsorPosition;


		// Direction to repulsor
		CFVec2 vDirection(vDisplacement);
		vDirection.Normalise();


		// Distance to repulsor
		float fDistance(vDisplacement.Magnitude());
		fDistance -= fRepulsorRadius;
		fDistance -= m_fRadius;


		// Within repulsion radius
		if (fDistance < fRepulsionRadius)
		{
			// How far into repulsion radius
			float fRepulsionRatio = 1.0f - (fDistance / fRepulsionRadius);


			// Opposite direction
			CFVec2 vOppositeDirection(vDirection);
			vOppositeDirection.Normalise();

				
			// Apply force opposite direction to repulsor
			vRepulseForce = vOppositeDirection;
			vRepulseForce *= m_aRepulsors[j]->GetMultiplier();
			vRepulseForce *= fRepulsionRatio;


			_vrRepelForce += vRepulseForce;
		}
	}
}





void
CMole::ComputeWallForce(CFVec2& _vrRepelForce)
{
	for (unsigned int i = 0; i < MAX_WHISKER; ++ i)
	{
		CFVec2 vLineRepelForce;
		CWall* pClosestWall = 0;
		float fImpactTime = 0;
		bool bIntersected = FindWiskerClosestWall(static_cast<EWhisker>(i), pClosestWall, fImpactTime);


		if (bIntersected)
		{
			// Time radio
			float fTimeRatio = 1.0f - (fImpactTime / 1.0f);
			fTimeRatio *= 1.8f;


			if (fTimeRatio > 1.0f)
			{
				fTimeRatio = 1.0f;
			}


			CFVec2 vPointDisplacement(pClosestWall->GetStartPoint());
			vPointDisplacement -= pClosestWall->GetEndPoint();


			// Line direction
			CFVec2 vLineDirection(vPointDisplacement);
			vLineDirection.Normalise();


			// Get Normal
			CFVec2 vLineNormal(pClosestWall->GetNormal());
			vLineNormal.Normalise();


			CFVec2 vStart = pClosestWall->GetStartPoint();
			CFVec2 vEnd = pClosestWall->GetEndPoint();


			// Which side is the cat on
			float fPerpendicularDot = (vEnd.X() - vStart.X()) * (m_vPosition.Y() - vStart.Y()) - (vEnd.Y() - vStart.Y()) * (m_vPosition.X() - vStart.X());


			bool bOnNormalSide = (fPerpendicularDot > 0);

			
			// Cat going against the normal
			if (bOnNormalSide)
			{
				// Cat has to be on normal side
				vLineRepelForce = vLineNormal;
				vLineRepelForce.RotateZ(fDegsToRad(180.0f));
			}


			// Cat going towards normal
			else
			{
				// Cat has to be not on normal side
				vLineRepelForce = vLineNormal;
			}


			// Scale based on collision time
			//vLineRepelForce *= 1.25f;
			vLineRepelForce *= fTimeRatio;


			_vrRepelForce += vLineRepelForce;
			m_pWhiskers[i].SetColour(0xFFFF0000);
		}
	}
}





bool
CMole::FindWiskerClosestWall(EWhisker _eWhisker, CWall*& _prClosestWall, float& _frImpactTime)
{
	CWall* pClosestWall = 0;
	float fSmallestImpactTime = 10.0f;
	bool bIntersected = false;


	// Get Walls
	CWall* pWalls = 0;
	int iNumWalls = 0;	


	CMMMContext& rMMMContext = CMMMContext::GetInstance();
	rMMMContext.GetWalls(pWalls, iNumWalls);


	// Check for collisions
	for (int i = 0; i < iNumWalls; ++ i)
	{
		const CFVec2& vWhiskerOrigin = m_pWhiskers[_eWhisker].GetOriginPoint();
		const CFVec2& vWhiskerEnd = m_pWhiskers[_eWhisker].GetEndPoint();
		float fImpactTime = 0.0f;


		if (pWalls[i].GetLineIntersectionTime(vWhiskerOrigin, vWhiskerEnd - vWhiskerOrigin, fImpactTime))
		{
			// Is collision
			if (fImpactTime >= 0.0f && fImpactTime <= 1.0f )
			{
				if (fImpactTime < fSmallestImpactTime)
				{
					pClosestWall = &pWalls[i];
					fSmallestImpactTime = fImpactTime;


					bIntersected = true;
				}
			}
		}
	}


	_prClosestWall = pClosestWall;
	_frImpactTime = fSmallestImpactTime;


	return (bIntersected);
}



#endif






/********************************
            Private
*********************************/





bool
CMole::InitialiseSprite(const char* _kcpSpriteTexture)
{
	// Clean up
	FW_RELEASE(m_pSprite);


	bool bReturn = false;


	m_pSprite = CMMMContext::GetInstance().GetSpriteM()->FindResouceById( GDE::CResourceId(_kcpSpriteTexture) );


	if (m_pSprite == 0)
	{
		GDE::CTexture* pTexture = CMMMContext::GetInstance().GetRenderer()->GetTextureManager().LoadResource(_kcpSpriteTexture);


		if (_kcpSpriteTexture == 0)
		{
			assert_now( "Unable to find Image \"%s\" for Mole", _kcpSpriteTexture );
		}
		else
		{
			GDE::SFRect TexRect;
			TexRect.m_fX = 0.0f;
			TexRect.m_fY = 0.0f;
			TexRect.m_fWidth = static_cast<float>(pTexture->GetWidth());
			TexRect.m_fHeight = static_cast<float>(pTexture->GetHeight());


			m_pSprite = CMMMContext::GetInstance().GetSpriteM()->CreateSprite( GDE::CResourceId( _kcpSpriteTexture ), pTexture, TexRect );
			m_pSprite->SetHotSpot( TexRect.m_fWidth*0.5f, TexRect.m_fHeight*0.5f );


			pTexture->Release();	// now held inside the sprite.


			bReturn = true;
		}
	}
	else
	{
		bReturn = true;
	}


	return (bReturn);
}




#ifndef _EDITOR


bool
CMole::IntiailiseWhiskers()
{
	bool bReturn = true;


	// Clean up
	FW_ADELETE(m_pWhiskers);


	m_pWhiskers = new CWhisker[MAX_WHISKER];


	// Rotation offsetss
	m_pWhiskers[WHISKER_LEFT].Initialise(fDegsToRad(-45));
	m_pWhiskers[WHISKER_RIGHT].Initialise(fDegsToRad(45));


	// Length
	m_pWhiskers[WHISKER_LEFT].SetLength(40);
	m_pWhiskers[WHISKER_RIGHT].SetLength(40);





	return (bReturn);
}


#endif





void
CMole::Deinitialise()
{
	FW_RELEASE(m_pSprite);

#ifndef _EDITOR
	FW_ADELETE(m_pWhiskers);
#endif
}