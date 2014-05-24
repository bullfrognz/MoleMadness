//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  (c) 2011 Bryce Booth
//
//  File Name   :   Task.h
//  Description :   --------------------------
//
//  Author      :   Bryce Booth
//  Mail        :   bryce.booth@mediadesign.school.nz
//


#pragma once


#ifndef __Task2dawd_H__
#define __Task2dawd_H__


// Local Includes
#include "Common.h"
#include "FVector2.h"


// Library Includes
#include <vector>


// Types


// Prototypes
class CAcorn;


class CTask
{

	// Member Types
public:


	enum EType
	{
		INVALID_TYPE = -1,

		TYPE_EXPLORE,
		TYPE_PATROL,
		TYPE_SNOOZE,
		TYPE_HUNT,
		TYPE_STUCK
	};


	static char* s_cpTaskName[];


	// Member Functions
public:


	 CTask();
	~CTask();


	bool Initialise(EType _eType);
	void Update(float _fDeltaTick);


	void PathPopBack();
	void SetNodeIdsPath(const std::vector<int>& _karPath);
	const std::vector<int>& GetNodeIdsPath() const;


	// Inline Functions
	inline void ResetTimeElapsed();
	inline void SetPathUpdateTimer(float _fTime);
	inline void DecrementSnoozeTime(float _fAmount);
	inline void DecrementStuckTime(float _fTime);
	inline void ClearTarget();


	inline void SetAcorn(CAcorn* _pAcorn);
	inline void SetTargetNodeId(int _iNodeId);
	inline void SetSnoozeTimeLeft(float _fTime);
	inline void SetStuckTimeLeft(float _fTime);


	inline EType GetType() const;
	inline CAcorn* GetAcorn();
	inline int GetTargetNodeId() const;
	inline float GetSnoozeTimeLeft() const;
	inline float GetStuckTimeLeft() const;
	inline float GetTimeElapsed() const;
	inline float GetPathUpdateTimer() const;


	inline bool HasAcorn() const;
	inline bool HasTargetNode() const;


protected:


private:


	void Deinitialise();


	// Member Variables
protected:


private:


	CAcorn* m_pAcorn;


	EType m_eType;


	float m_fSnoozeTimeLeft;
	float m_fTimeElapsed;
	float m_fPathUpdateTimer;
	float m_fStuckTimeLeft;


	int m_iTargetNodeId;


	std::vector<int> m_aNodeIdsPath;


};


#include "Task.inl"


#endif //__Task_H__