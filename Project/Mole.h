//
//  Media Design School
//  Auckland
//  New Zealand
//
//  File Name   :   Mole.h
//  Description :   --------------------------
//
//  Copyright © 2004-2010 Media Design School. All Rights Reserved.
//


#pragma once


#ifndef __Mole_H__
#define __Mole_H__


// Library Includes
#include <windows.h>
#include <stack>
#include <vector>
#include <map>


// Local Includes
#include "FVector2.h"
#include "Whisker.h"
#include "Path.h"
#include "Task.h"
#include "Repulsor.h"


// Types
namespace GDE
{
	class CSprite;
};


// Prototypes
class CCircle;
class CBox2D;
class CSquirrel;
class CAcorn;
class CPath;
class CWall;


class CMole
{

	// Member Types
public:


protected:


	enum ESettings
	{
		PARTROL_RADIUS = 100,

		MOVEMENT_VELOCITY = 100,	// Pixels
		ROTATION_VELOCITY = 150,	// Degrees
		SLEEP_TIME		  = 5,


		DIFFICULTY_MOVEMENT_VELOCITY_INCR = 12,
		DIFFICULTY_ROTATION_VELOCITY_INCR = 15,
		DIFFICULTY_SLEEP_TIME_DESC = 1
	};


	enum EWhisker
	{
		MIN_WHISKER = -1,

		WHISKER_LEFT,
		WHISKER_RIGHT,

		MAX_WHISKER
	};


	// Member Functions
public:


	 CMole();
	~CMole();


	static CPath* GetPath();


	bool Initialise(const CFVec2& _kvrPosition, const char* _kcpSpriteTexture, float _fHeading);
	void Update(float _fTimeDelta, CSquirrel& _rSquirrel);
	void Draw(bool _bDrawKnownAcorns = false);


	void NotifyDisturbance(const CCircle& _krCircle);
	void NotifyAcorn(CAcorn* _pAcorn);
	static void NotifyAcornDropped(CAcorn* _pAcorn);


	bool Intersects(const CCircle& _krCircle) const;
	bool Intersects(const CBox2D& _krBox) const;


	void UpdateDifficulty(int _iLevelId);


	// Inline Functions
	inline void SetHighlighted(bool _bHighlighted);


	inline const CFVec2& GetPosition() const;
	inline float GetRadius() const;
	inline float GetHeading() const;
	inline bool IsHighlighted() const;
	inline CRepulsor* GetRepulsor();


	static inline const CPath* GetSharedPath();


	void BagsAcorn();
	bool DoesHavePatrolTask();


	void AppendExploreTask();
	void AppendPatrolTask(CAcorn* _pAcorn);
	void AppendSnoozeTask(float _fTime);
	void AppendHuntTask(const CFVec2& _kvrTargetPosition);
	void AppendStuckTask(float _fTime);


protected:

	void ProcessStuckManager(float _fTimeDelta);
	void ProcessExplore(float _fTimeDelta, CTask& _rTask);
	void ProcessPatrol(float _fTimeDelta, CTask& _rTask);
	void ProcessSnooze(float _fTimeDelta, CTask& _rTask);
	void ProcessHunt(float _fTimeDelta, CTask& _rTask);
	void ProcessStuck(float _fTimeDelta, CTask& _rTask);


	void UpdateTaskTargetPath(float _fTimeDelta, CTask& _rTask);
	void UpdateRotation(float _fTimeDelta, const CFVec2& _kvrForce);
	void UpdatePosition(float _fTimeDelta);
	void UpdateWhiskers();


	void DrawWhiskers() const;
	void DrawTaskPathTarget(CTask& _rTask) const;
	void DrawTargetAcorn(const CAcorn* _kpAcorn) const;
	void DrawTaskType() const;
	void DrawSteeringForce() const;
	void DrawKnownAcorns() const;


	void ComputeSteerForce(int _iTargetNodeId, CFVec2& _vrSteerForce);
	void ComputeSeekForce(CFVec2& _vrSeekForce, int _iTargetNodeId);
	void ComputeRepulsorForce(CFVec2& _vrRepelForce);
	void ComputeWallForce(CFVec2& _vrRepelForce);


	bool FindWiskerClosestWall(EWhisker _eWhisker, CWall*& _prClosestWall, float& _frImpactTime);


private:


	bool InitialiseSprite(const char* _kcpSpriteTexture);
	bool IntiailiseWhiskers();


	void Deinitialise();


	CMole(const CMole& _krMole);
	CMole& operator = (const CMole& _krMole);


	// Member Variables
protected:


private:


	CFVec2 m_vPosition;
	CFVec2 m_vLastPosition;
	CFVec2 m_vTranslationTracker;
	CFVec2 m_vLastSteeringForce;


	GDE::CSprite* m_pSprite;
	CWhisker* m_pWhiskers;
	CRepulsor m_Repulsor;


	float m_fTranslationTimer;
	float m_fRadius;
	float m_fHeading;
	float m_fMovementVelocity;
	float m_fRotationVelocity;


	int m_iDifficultyLevel;
	

	bool m_bHighlighted;
	bool m_bHasPatrolTask;


#ifndef _EDITOR
	std::stack<CTask> m_Tasks;
#endif


	//static 
	static int s_iReffCount;
	static CPath* s_pSharedPath;
	static std::map<CAcorn*, bool> s_mKnownAcorns;


};


#include "Mole.inl"


#endif //__Mole_H__
