//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  (c) 2011 Bryce Booth
//
//  File Name   :   Path.h
//  Description :   --------------------------
//
//  Author      :   Bryce Booth
//  Mail        :   bryce.booth@mediadesign.school.nz
//


#pragma once


#ifndef __Path_H__
#define __Path_H__


// Local Includes
#include "FVector2.h"


// Library Includes
#include <windows.h>
#include <vector>


// Types
namespace Path
{
	enum EEdge
	{
		INVALID_EDGE = -1,

		EDGE_NORTH,
		EDGE_EAST,
		EDGE_SOUTH,
		EDGE_WEST,

		EDGE_NORTH_EAST,
		EDGE_SOUTH_EAST,
		EDGE_SOUTH_WEST,
		EDGE_NORTH_WEST,

		MAX_EDGE
	};


	enum EConstants
	{
		EDGEWEIGHT_HORIZONTAL = 10,
		EDGEWEIGHT_VERTICAL	  = 14,

		NODE_RADIUS = 6
	};


	struct TEdge
	{
		TEdge::TEdge()
		{
			SetInvalid();
		}

		void SetInvalid()
		{
			iNodeId = -1;
			iWeight = 0;
		}

		bool IsValid() const
		{
			return (iNodeId != -1);
		}

		int iNodeId;
		int iWeight;
	};


	struct TNode
	{
		TNode::TNode()
		: iX(0)
		, iY(0)
		, iColour(0xAAAAAAAA)
		, bBlocked(false)
		, bDiscovered(false)
		{
			// Empty
		}

		CFVec2 vPosition;

		int iX;
		int iY;
		int iColour;

		bool bBlocked;
		bool bDiscovered;

		TEdge m_tEdges[MAX_EDGE];
	};
}


// Prototypes


class CPath
{

	// Member Functions
public:


	 CPath();
	~CPath();


	bool Initialise(float _fMapWidth, float _fMapHeight, float _fNodeOffset);
	void Draw() const;

	#ifndef _EDITOR
	void NotifyAccessablePosition(const CFVec2& _kvrPosition, float _fRadius);


	bool FindUndiscoveredNode(const CFVec2& _kvrPosition, float _fRadius, float _fHeading, int& _irTargetNodeId);
	bool FindPath(const CFVec2& _kvrPosition, int _iTargetNodeId, std::vector<int>& _arNodeIdPath);
	bool FindRandomAccesableNode(const CFVec2& _kvrPosition, float _fSearchRadius, int& _irRandomNodeId);
	void FindRandomAccesableNode(int& _irRandomNodeId);


	bool IsIntersectingNode(const CFVec2& _kvrPosition, float _fRadius, int _iNodeId);


	const Path::TNode& GetNode(const CFVec2& _kvrPosition) const;


	// Inline Functions
	inline const Path::TNode& GetNode(int _iId) const;
	inline int GenerateNodeId(int _iIndexX, int _iIndexY);
	inline int GenerateNodeId(const Path::TNode& _ktrNode);
	inline bool HasUndiscoveredNodesLeft() const;



#endif


protected:


	void DrawNodes() const;

#ifndef _EDITOR
	bool DiscoverNode(int _iNodeId);


	bool CheckNodeBlocked(int _iNodeId);


	int GenerateHeristic(int _iNodeId, int _iTargetNodeId);


	void FindNodesInRadius(const CFVec2& _kvrPosition, float _fRadius, std::vector<int>& _arNodeIds);
#endif


private:


	bool InitialiseNodes();
	bool IntiailiseNodePositions();
	bool InitialiseNodeEdges();


	void SetupNodePosition(int _iX, int _iY);
	void SetupNodeEdges(int _iX, int _iY);

	void Deinitialise();



	CPath(const CPath& _krPath);
	CPath& operator = (const CPath& _krPath);


	// Member Variables
protected:


private:


	Path::TNode* m_tpNodes;


	float m_fMapWidth;
	float m_fMapHeight;
	float m_fNodeOffset;


	int m_iNumNodesX;
	int m_iNumNodesY;


	bool m_bHasUndiscoveredNodes;


};


#include "Path.inl"


#endif //__Path_H__