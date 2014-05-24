//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  (c) 2011 Bryce Booth
//
//  File Name :   Whisker.cpp
//
//  Author    :   Bryce Booth
//  Mail      :   bryce.booth@mediadesign.school.nz
//


// Library Includes
#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"
#include "GDE/Renderer/Vertex.h"


#include "MMMContext.h"


// Local Includes


// This Include
#include "Whisker.h"


// Static Variables


// Implementation


/********************************
            Public
*********************************/





/*---------------------------------------------------------------------------------------------------------------------------
*
* Whisker constructors
*
*---------------------------------------------------------------------------------------------------------------------------*/

CWhisker::CWhisker()
: m_fLength(0)
, m_fHeadingOfffset(0)
, m_uiColour(0xFF000000)
{
	//Empty
}





/*---------------------------------------------------------------------------------------------------------------------------
*
* Whisker deconstructor
*
*---------------------------------------------------------------------------------------------------------------------------*/

CWhisker::~CWhisker()
{
	Deinitialise();
}





bool
CWhisker::Initialise(float _fHeadingOffset)
{
	m_fHeadingOfffset = _fHeadingOffset;


	return (true);
}





void
CWhisker::Update(const CFVec2& _kvrPosition, float _fParentHeading)
{
	m_vOriginPoint = _kvrPosition;


	// Compute new end point
	m_vEndPoint.X(0);
	m_vEndPoint.Y(-1);
	m_vEndPoint.Rotate(m_fHeadingOfffset + _fParentHeading);
	m_vEndPoint *= m_fLength;
	m_vEndPoint +=  m_vOriginPoint;
}





void
CWhisker::Draw()
{
	GDE::CRenderer* pRenderer = CMMMContext::GetInstance().GetRenderer();


	// Colour
	GDE::SVertex Vertex[2];
	Vertex[0].m_uColour = m_uiColour;
	Vertex[1].m_uColour = m_uiColour;


	// Origin vertex
	Vertex[0].m_fX = m_vOriginPoint.X();
	Vertex[0].m_fY = m_vOriginPoint.Y();
	Vertex[0].m_fZ = 0.0f;


	// End vertex
	Vertex[1].m_fX = m_vEndPoint.X();
	Vertex[1].m_fY = m_vEndPoint.Y();
	Vertex[1].m_fZ = 0.0f;


	pRenderer->DrawPrimitives(GDE::PT_LineList, 1, Vertex);

	
	// Reset colour
	m_uiColour = 0xAA000000;
}





/********************************
            Protected
*********************************/






/********************************
            Private
*********************************/





void
CWhisker::Deinitialise()
{
	//Empty
}