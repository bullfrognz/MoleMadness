//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  (c) 2011 Bryce Booth
//
//  File Name :   Path.cpp
//
//  Author    :   Bryce Booth
//  Mail      :   bryce.booth@mediadesign.school.nz
//


// Local Includes
#include "Circle.h"
#include "Box2D.h"
#include "MMMContext.h"
#include "Wall.h"


// Library Includes
#include <cmath>
#include <map>
#include <list>
#include <algorithm>
#include <iostream>
#include <assert.h>


#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"
#include "GDE/Renderer/Vertex.h"


// This Include
#include "Path.h"


// Static Variables


// Implementation
using namespace Path;


/********************************
            Public
*********************************/





/*---------------------------------------------------------------------------------------------------------------------------
*
* Path constructors
*
*---------------------------------------------------------------------------------------------------------------------------*/

CPath::CPath()
: m_tpNodes(0)
, m_fMapWidth(0)
, m_fMapHeight(0)
, m_fNodeOffset(0)
, m_iNumNodesX(0)
, m_iNumNodesY(0)
, m_bHasUndiscoveredNodes(true)
{
	//Empty
}





/*---------------------------------------------------------------------------------------------------------------------------
*
* Path deconstructor
*
*---------------------------------------------------------------------------------------------------------------------------*/

CPath::~CPath()
{
	Deinitialise();
}





/*---------------------------------------------------------------------------------------------------------------------------
*
*	Initialises the path object.
*
* 	@param	_iMapWidth		Pixel width of game map. 
* 	@param	_iMapHeight		Pixel height of game map.
* 	@param	_iNodeDensity	Minimum distance between nodes. 
*
*	@return	
*
*---------------------------------------------------------------------------------------------------------------------------*/

bool
CPath::Initialise(float _fMapWidth, float _fMapHeight, float _fNodeOffset)
{
	m_fMapWidth = _fMapWidth;
	m_fMapHeight = _fMapHeight;
	m_fNodeOffset = _fNodeOffset;
	m_iNumNodesX = static_cast<int>(m_fMapWidth  / m_fNodeOffset);
	m_iNumNodesY = static_cast<int>(m_fMapHeight / m_fNodeOffset);


	InitialiseNodes();


	return (true);
}




void
CPath::Draw() const
{
	DrawNodes();
}



#ifndef _EDITOR
void
CPath::NotifyAccessablePosition(const CFVec2& _kvrPosition, float _fRadius)
{
	CCircle NotifierCircle;
	NotifierCircle.Initialise(_kvrPosition, _fRadius);
	CBox2D NodeBox;
	CFVec2 vExtents(m_fNodeOffset / 2.0f, m_fNodeOffset / 2.0f);


	for (int iY = 0; iY < m_iNumNodesY; ++ iY)
	{
		for (int iX = 0; iX < m_iNumNodesX; ++ iX)
		{
			const int kiNodeId = iY * m_iNumNodesX + iX;


			NodeBox.InitExtents(m_tpNodes[kiNodeId].vPosition, vExtents);


			if (NodeBox.Intersects(NotifierCircle))
			{
				if (!m_tpNodes[kiNodeId].bDiscovered)
				{
					DiscoverNode(kiNodeId);
				}
			}
			else
			{
				// Do nothing
			}
		}
	}
}





bool
CPath::FindUndiscoveredNode(const CFVec2& _kvrPosition, float _fRadius, float _fHeading, int& _irTargetNodeId)
{
	bool bReturn = true;


	if (!m_bHasUndiscoveredNodes)
	{
		return (false);
	}


	// Find nodes intersecting position & radius
	std::vector<int> aIntersectingNodesIds;


	FindNodesInRadius(_kvrPosition, _fRadius, aIntersectingNodesIds);


	// Find unqiue undiscovered edges
	std::vector<int>::const_iterator kCurrentNodeId = aIntersectingNodesIds.begin();
	std::vector<int>::const_iterator kEndNodeId = aIntersectingNodesIds.end();
	std::map<int, bool> mUndiscoveredNodeIds;


	for (kCurrentNodeId; kCurrentNodeId != kEndNodeId; ++ kCurrentNodeId)
	{
		const TNode& krNode = m_tpNodes[(*kCurrentNodeId)];
		const TEdge* kpNodeEdges = krNode.m_tEdges;


		for (int iEdge = 0; iEdge < MAX_EDGE; ++ iEdge)
		{
			const int kiNodeId = kpNodeEdges[iEdge].iNodeId;


			if (kiNodeId != -1)
			{
				if (!m_tpNodes[kiNodeId].bDiscovered)
				{
					mUndiscoveredNodeIds.insert( std::pair<int, bool>(kpNodeEdges[iEdge].iNodeId, true) );
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
	}


	if (mUndiscoveredNodeIds.size() > 0)
	{
		// Find best node based on dot
		std::map<int, bool>::const_iterator kCurrentNode = mUndiscoveredNodeIds.begin();
		std::map<int, bool>::const_iterator kEndNode = mUndiscoveredNodeIds.end();
		float fBestNodeDot = 4.0f;
		int iBestNodeId = -1;


		for (kCurrentNode; kCurrentNode != kEndNode; ++ kCurrentNode)
		{
			TNode* tpNode = &m_tpNodes[(*kCurrentNode).first];


			CFVec2 vHeading(0.0f, -1.0f);
			vHeading.Rotate(_fHeading);


			CFVec2 vDisplacement(_kvrPosition);
			vDisplacement -= tpNode->vPosition;
			vDisplacement.Normalise();


			float fDot = vDisplacement.DotProduct(vHeading);


			if (fDot < fBestNodeDot)
			{
				fBestNodeDot = fDot;
				iBestNodeId = (*kCurrentNode).first;
			}
			else
			{
				// Do nothing
			}
		}


		_irTargetNodeId = iBestNodeId;
	}
	else
	{
		// Find closest undiscovered edge for discovered non-blocked nodes 
		float fSmallestDistance = 1000000.0f;
		int iClosestNodeId = -1;


		for (int iY = 0; iY < m_iNumNodesY; ++ iY)
		{
			for (int iX = 0; iX < m_iNumNodesX; ++ iX)
			{
				const int kiNodeId = iY * m_iNumNodesX + iX;


				if (m_tpNodes[kiNodeId].bDiscovered &&
					!m_tpNodes[kiNodeId].bBlocked)
				{
					const TEdge* kpEdges = m_tpNodes[kiNodeId].m_tEdges;


					for (int iEdge = 0; iEdge < MAX_EDGE; ++ iEdge)
					{
						const int kiEdgeNodeId = kpEdges[iEdge].iNodeId;
	

						if (kiEdgeNodeId != -1 &&
							!m_tpNodes[kiEdgeNodeId].bDiscovered &&
							!m_tpNodes[kiEdgeNodeId].bBlocked)
						{
							float fDistance = m_tpNodes[kiEdgeNodeId].vPosition.SquareDistance(_kvrPosition);
							fDistance = sqrt(fDistance);
							

							if (fDistance < fSmallestDistance)
							{
								iClosestNodeId = kiEdgeNodeId;
								fSmallestDistance = fDistance;
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
				}
				else
				{
					// Do nothing
				}
			}
		}


		// Check is an undiscovered node was found
		if (iClosestNodeId != -1)
		{
			_irTargetNodeId = iClosestNodeId;
		}
		else
		{
			m_bHasUndiscoveredNodes = false;
		}
	}


	return (bReturn);
}





bool
CPath::FindPath(const CFVec2& _kvrPosition, int _iTargetNodeId, std::vector<int>& _arNodeIdPath)
{
	_arNodeIdPath.clear();


	// A * Path find to the location
	struct TRoute
	{
		TRoute::TRoute()
		{
			memset(this, 0, sizeof(TRoute));
		}

		TRoute* kpParentRoute;

		int iNodeId;
		int iGScore;
		int iHScore;
		int iFScore;
	};


	TRoute* tpPathRoute = 0;
	std::vector<TRoute*> aOpenList;
	std::vector<int>     aCloseList;
	std::vector<TRoute*> aRouteTracker;


	// Add start route
	const TNode& krStartNode = GetNode(_kvrPosition);


	TRoute* tpStartRoute = new TRoute;
	tpStartRoute->iNodeId = GenerateNodeId(krStartNode);
	//tpStartRoute->iNodeId = -1;


	/*
	// Check start node is blocked
	if (krStartNode.bBlocked)
	{
		CWall* pWalls = 0;
		int iNumWalls = 0;	


		CMMMContext& rMMMContext = CMMMContext::GetInstance();
		rMMMContext.GetWalls(pWalls, iNumWalls);


		// Find a edge that we can use instead thats not blocked
		const TEdge* kpEdges = krStartNode.m_tEdges;
		

		for (int i = 0; i < MAX_EDGE; ++ i)
		{
			if (kpEdges[i].iNodeId != -1 &&
				!m_tpNodes[kpEdges[i].iNodeId].bBlocked)
			{
				bool bEdgeValid = true;


				for (int j = 0; j < iNumWalls; ++ j)
				{
					float fTime = 0;


					if (pWalls[j].GetLineIntersectionTime(krStartNode.vPosition, m_tpNodes[kpEdges[i].iNodeId].vPosition - krStartNode.vPosition, fTime))
					{
						if (fTime >= 0.0f && fTime <= 1.0f)
						{
							bEdgeValid = false;
							break;
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


				if (bEdgeValid)
				{
					tpStartRoute->iNodeId = kpEdges[i].iNodeId;
					break;
				}
			}
			else
			{
				// Do nothing
			}
		}
	}
	else
	{
		tpStartRoute->iNodeId = GenerateNodeId(krStartNode);
	}



	assert(tpStartRoute->iNodeId != -1);

	*/
	

	aOpenList.push_back(tpStartRoute);


	// Find path
	bool bSearching = true;
	bool bPathFound = false;


	while (bSearching)
	{
		// Stop when you: Fail to find the target square, and the open list is empty
		if (aOpenList.size() == 0)
		{
			// Could not find path!, Set node to Discovered, blocked
			m_tpNodes[_iTargetNodeId].bDiscovered = true;
			m_tpNodes[_iTargetNodeId].bBlocked = true;
			//assert(false);
			bSearching = false;
			continue;
		}


		// a) Look for the lowest F cost route on the open list. We refer to this as the current route.
		int iLowestFScoreIndex = 0;
		int iLowestFScore = -1;


		for (unsigned int i = 0; i < aOpenList.size(); ++ i)
		{
			if (iLowestFScore == -1 ||
				aOpenList[i]->iFScore < iLowestFScore)
			{
				iLowestFScoreIndex = i;
				iLowestFScore = aOpenList[i]->iFScore;
			}
		}


		TRoute* tpCurrentRoute = aOpenList[iLowestFScoreIndex];


		// b) Switch it to the closed list.
		aOpenList.erase(aOpenList.begin() + iLowestFScoreIndex);
		aCloseList.push_back(tpCurrentRoute->iNodeId);


		// Stop when you: Add the target square to the closed list
		if (tpCurrentRoute->iNodeId == _iTargetNodeId)
		{
			tpPathRoute = tpCurrentRoute;
			bSearching = false;
			bPathFound = true;
			continue;
		}


		// c) For each of the 8 squares adjacent to this current square …
		const TEdge* ktpEdges = m_tpNodes[tpCurrentRoute->iNodeId].m_tEdges;


		for (int iEdge = 0; iEdge < MAX_EDGE; ++ iEdge)
		{
			const TEdge& krEdge = ktpEdges[iEdge];


			// If edge invalid, is not walkable or if it is on the closed list, ignore it.
			if (!krEdge.IsValid() ||
				(m_tpNodes[krEdge.iNodeId].bBlocked && krEdge.iNodeId != _iTargetNodeId) ||
				std::find(aCloseList.begin(), aCloseList.end(), krEdge.iNodeId) != aCloseList.end())
			{
				continue;
			}


			// If it isn't on the open list,
			int iOpenListIndex = -1;
			bool bInOpenList = false;


			for (unsigned int i = 0; i < aOpenList.size(); ++ i)
			{
				if (aOpenList[i]->iNodeId == krEdge.iNodeId)
				{
					iOpenListIndex = i;
					bInOpenList = true;
					break;
				}
			}

			
			// add it to the open list. Else
			if (!bInOpenList)
			{
				TRoute* tpEdgeRoute = new TRoute;
				tpEdgeRoute->iNodeId = krEdge.iNodeId;
				tpEdgeRoute->kpParentRoute = tpCurrentRoute;
				tpEdgeRoute->iGScore = krEdge.iWeight + tpCurrentRoute->iGScore;
				tpEdgeRoute->iHScore = GenerateHeristic(krEdge.iNodeId, _iTargetNodeId);
				tpEdgeRoute->iFScore = tpEdgeRoute->iGScore + tpEdgeRoute->iHScore;


				aOpenList.push_back(tpEdgeRoute);
				aRouteTracker.push_back(tpEdgeRoute);
			}


			// check to see if this path to that square is better.
			else if (tpCurrentRoute->iGScore < aOpenList[iOpenListIndex]->iGScore)
			{
				aOpenList[iOpenListIndex]->kpParentRoute = tpCurrentRoute;
				aOpenList[iOpenListIndex]->iGScore = ktpEdges[iEdge].iWeight + tpCurrentRoute->iGScore;
				aOpenList[iOpenListIndex]->iFScore = aOpenList[iOpenListIndex]->iGScore + aOpenList[iOpenListIndex]->iHScore;
			}
			else
			{
				// Do nothing
			}
		}
	}


	// Create node id path
	if (bPathFound)
	{
		do
		{
			_arNodeIdPath.push_back(tpPathRoute->iNodeId);

			
			if (tpPathRoute->kpParentRoute != 0)
			{
				tpPathRoute = tpPathRoute->kpParentRoute;
			}
		}
		while (tpPathRoute->kpParentRoute != 0);
	}


	// Clean up
	for (unsigned int i = 0; i < aRouteTracker.size(); ++ i)
	{
		delete aRouteTracker[i];
		aRouteTracker[i] = 0;
	}


	delete tpStartRoute;
	tpStartRoute = 0;
	

	return (bPathFound);
}





bool
CPath::FindRandomAccesableNode(const CFVec2& _kvrPosition, float _fSearchRadius, int& _irRandomNodeId)
{
	//const TNode& krNode = GetNode(_kvrPosition);
	bool bNodeFound = false;


	std::vector<int> aNodeList;


	// Get all nodes withing distance
	for (int iY = 0; iY < m_iNumNodesY; ++ iY)
	{
		for (int iX = 0; iX < m_iNumNodesX; ++ iX)
		{
			int iNodeId = GenerateNodeId(iX, iY);


			if (!m_tpNodes[iNodeId].bBlocked)
			{
				float fDistance = sqrt(m_tpNodes[iNodeId].vPosition.SquareDistance(_kvrPosition));
							

				if (fDistance <= _fSearchRadius)
				{
					aNodeList.push_back(iNodeId);
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
	}


	if (aNodeList.size() > 0)
	{
		// Pick random node
		int iRandomNodeIndex = rand() % aNodeList.size();


		_irRandomNodeId = aNodeList[iRandomNodeIndex];


		bNodeFound = true;
	}


	return (bNodeFound);
}





void
CPath::FindRandomAccesableNode(int& _irRandomNodeId)
{
	const int kiMaxNodeId = GenerateNodeId(m_iNumNodesX - 1, m_iNumNodesY - 1);
	int iRandomNodeId = -1;


	do
	{
		int iRand = rand() % kiMaxNodeId;


		if (!m_tpNodes[iRand].bBlocked)
		{
			iRandomNodeId = iRand;
		}
		else
		{
			// Do nothing
		}
	}
	while (iRandomNodeId == -1);


	_irRandomNodeId = iRandomNodeId;
}





bool
CPath::IsIntersectingNode(const CFVec2& _kvrPosition, float _fRadius, int _iNodeId)
{
	CCircle RequesterCircle;
	RequesterCircle.Initialise(_kvrPosition, _fRadius);
	CBox2D NodeBox;
	CFVec2 vExtents(m_fNodeOffset / 2.0f, m_fNodeOffset / 2.0f);
	bool bReturn = false;


	NodeBox.InitExtents(m_tpNodes[_iNodeId].vPosition, vExtents);


	if (NodeBox.Intersects(RequesterCircle))
	{
		bReturn = true;
	}
	else
	{
		// Do nothing
	}


	return (bReturn);
}





const Path::TNode&
CPath::GetNode(const CFVec2& _kvrPosition) const
{
	// Find closest node to 
	std::vector<TNode*> aIntersectingNodes;
	const TNode* tpClosestNode = 0;
	float fClosestNodeDistance = -1.0f;
	


	for (int iY = 0; iY < m_iNumNodesY; ++ iY)
	{
		for (int iX = 0; iX < m_iNumNodesX; ++ iX)
		{
			const int kiNodeId = iY * m_iNumNodesX + iX;


			float fDistance = m_tpNodes[kiNodeId].vPosition.SquareDistance(_kvrPosition);


			if (fClosestNodeDistance == -1.0f ||
				fDistance < fClosestNodeDistance)
			{
				tpClosestNode = &m_tpNodes[kiNodeId];
				fClosestNodeDistance = fDistance;
			}
		}
	}


	return (*tpClosestNode);
}






#endif
/********************************
            Protected
*********************************/





void
CPath::DrawNodes() const
{
	CBox2D TempBox;
	CFVec2 vExtents(m_fNodeOffset / 2.0f - 1.0f, m_fNodeOffset / 2.0f - 1.0f);
	//CMMMContext& rMMMContext = CMMMContext::GetInstance();
	//GDE::CRenderer* pRenderer = rMMMContext.GetRenderer();
	

	// Draw active nodes
	for (int iY = 0; iY < m_iNumNodesY; ++ iY)
	{
		for (int iX = 0; iX < m_iNumNodesX; ++ iX)
		{
			unsigned int uiNodeId = iY * m_iNumNodesX + iX;


			TempBox.InitExtents(m_tpNodes[uiNodeId].vPosition, vExtents);
			TempBox.Draw(m_tpNodes[uiNodeId].iColour);
		}
	}
}





#ifndef _EDITOR
bool
CPath::DiscoverNode(int _iNodeId)
{
	bool bReturn = false;


	if (!m_tpNodes[_iNodeId].bDiscovered)
	{
		if (CheckNodeBlocked(_iNodeId))
		{
			m_tpNodes[_iNodeId].bBlocked = true;
			m_tpNodes[_iNodeId].iColour = 0xFF0000FF;
		}
		else
		{
			m_tpNodes[_iNodeId].iColour = 0xFFFF0000;
		}


		m_tpNodes[_iNodeId].bDiscovered = true;
	}
	else
	{
		// Do nothing
	}


	return (bReturn);
}





bool
CPath::CheckNodeBlocked(int _iNodeId)
{
	CBox2D NodeBox;
	CFVec2 vExtents(m_fNodeOffset / 2.0f, m_fNodeOffset / 2.0f);
	bool bBlocked = false;


	// Get Walls
	CWall* pWalls = 0;
	int iNumWalls = 0;	


	CMMMContext& rMMMContext = CMMMContext::GetInstance();
	rMMMContext.GetWalls(pWalls, iNumWalls);


	for (int iWall = 0; iWall < iNumWalls; ++ iWall)
	{
		NodeBox.InitExtents(m_tpNodes[_iNodeId].vPosition, vExtents);


		if (NodeBox.OverlapsLineSegment(pWalls[iWall].GetEnd0(), pWalls[iWall].GetEnd1()))
		{
			bBlocked = true;


			break;
		}
		else
		{
			// Do nothing
		}
	}
	

	return (bBlocked);
}





int
CPath::GenerateHeristic(int _iNodeId, int _iTargetNodeId)
{
	// Get X Y difference
	int iDifferenceX = m_tpNodes[_iTargetNodeId].iX - m_tpNodes[_iNodeId].iX;
	int iDifferenceY = m_tpNodes[_iTargetNodeId].iY - m_tpNodes[_iNodeId].iY;


	int iHeuriestic = 0;
	iHeuriestic += abs(iDifferenceX * 10);
	iHeuriestic += abs(iDifferenceY * 10);


	return (iHeuriestic);
}





void
CPath::FindNodesInRadius(const CFVec2& _kvrPosition, float _fRadius, std::vector<int>& _arNodeIds)
{
	for (int iY = 0; iY < m_iNumNodesY; ++ iY)
	{
		for (int iX = 0; iX < m_iNumNodesX; ++ iX)
		{
			const int kiNodeId = iY * m_iNumNodesX + iX;


			if (!m_tpNodes[kiNodeId].bBlocked &&
				IsIntersectingNode(_kvrPosition, _fRadius, kiNodeId))
			{
				_arNodeIds.push_back(GenerateNodeId(m_tpNodes[kiNodeId]));
			}
			else
			{
				// Do nothing
			}
		}
	}
}





#endif
/********************************
            Private
*********************************/





bool
CPath::InitialiseNodes()
{
	const int kiTotalNodeCount = m_iNumNodesX * m_iNumNodesY;


	m_tpNodes = new Path::TNode[kiTotalNodeCount];


	for (int iY = 0; iY < m_iNumNodesY; ++ iY)
	{	
		for (int iX = 0; iX < m_iNumNodesX; ++ iX)
		{
			SetupNodePosition(iX, iY);
			SetupNodeEdges(iX, iY);
		}
	}


	return (true);
}





void
CPath::SetupNodePosition(int _iX, int _iY)
{
	const float kfPositionPadding = m_fNodeOffset / 2.0f;
	const float kfPositionY = (_iY * m_fNodeOffset) + kfPositionPadding;
	const float kfPositionX = (_iX * m_fNodeOffset) + kfPositionPadding;
	const int kiNodeId   = (_iY * m_iNumNodesX) + _iX;		


	m_tpNodes[kiNodeId].vPosition.X(kfPositionX);
	m_tpNodes[kiNodeId].vPosition.Y(kfPositionY);
	m_tpNodes[kiNodeId].iX = _iX;
	m_tpNodes[kiNodeId].iY = _iY;
}





void
CPath::SetupNodeEdges(int _iX, int _iY)
{
	const int kiNodeId = (_iY * m_iNumNodesX) + _iX;	


	// North
	if (_iY > 0)
	{
		m_tpNodes[kiNodeId].m_tEdges[EDGE_NORTH].iNodeId = kiNodeId - m_iNumNodesX;
		m_tpNodes[kiNodeId].m_tEdges[EDGE_NORTH].iWeight = EDGEWEIGHT_HORIZONTAL;
	}


	// West
	if (_iX > 0)
	{
		m_tpNodes[kiNodeId].m_tEdges[EDGE_WEST].iNodeId = kiNodeId - 1;
		m_tpNodes[kiNodeId].m_tEdges[EDGE_WEST].iWeight = EDGEWEIGHT_HORIZONTAL;
	}


	// South
	if (_iY < m_iNumNodesY - 1)
	{
		m_tpNodes[kiNodeId].m_tEdges[EDGE_SOUTH].iNodeId = kiNodeId + m_iNumNodesX;
		m_tpNodes[kiNodeId].m_tEdges[EDGE_SOUTH].iWeight = EDGEWEIGHT_HORIZONTAL;
	}


	// East
	if (_iX < m_iNumNodesX - 1)
	{
		m_tpNodes[kiNodeId].m_tEdges[EDGE_EAST].iNodeId = kiNodeId + 1;
		m_tpNodes[kiNodeId].m_tEdges[EDGE_EAST].iWeight = EDGEWEIGHT_HORIZONTAL;
	}


	// North East
	if (_iY > 0 &&
		_iX < m_iNumNodesX - 1)
	{
		m_tpNodes[kiNodeId].m_tEdges[EDGE_NORTH_EAST].iNodeId = kiNodeId - m_iNumNodesX + 1;
		m_tpNodes[kiNodeId].m_tEdges[EDGE_NORTH_EAST].iWeight = EDGEWEIGHT_VERTICAL;
	}


	// South East
	if (_iY < m_iNumNodesY - 1 &&
		_iX < m_iNumNodesX - 1)
	{
		m_tpNodes[kiNodeId].m_tEdges[EDGE_SOUTH_EAST].iNodeId = kiNodeId + m_iNumNodesX + 1;
		m_tpNodes[kiNodeId].m_tEdges[EDGE_SOUTH_EAST].iWeight = EDGEWEIGHT_VERTICAL;
	}


	// South West
	if (_iY < m_iNumNodesY - 1 &&
		_iX > 0)
	{
		m_tpNodes[kiNodeId].m_tEdges[EDGE_SOUTH_WEST].iNodeId = kiNodeId + m_iNumNodesX - 1;
		m_tpNodes[kiNodeId].m_tEdges[EDGE_SOUTH_WEST].iWeight = EDGEWEIGHT_VERTICAL;
	}


	// North West
	if (_iY > 0 &&
		_iX > 0)
	{
		m_tpNodes[kiNodeId].m_tEdges[EDGE_NORTH_WEST].iNodeId = kiNodeId - m_iNumNodesX - 1;
		m_tpNodes[kiNodeId].m_tEdges[EDGE_NORTH_WEST].iWeight = EDGEWEIGHT_VERTICAL;
	}
}


void
CPath::Deinitialise()
{
	delete[] m_tpNodes;
	m_tpNodes = 0;
}

