/**
 *	@file GDE\GDE\Renderer\Vertex.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _VERTEX_H__Included_1793287761
#define _VERTEX_H__Included_1793287761

namespace GDE
{

/**
 *	This vertex is a very simple type but the default.
 */
struct SVertex
{
	FLOAT32 m_fX;		//!< The X position.
	FLOAT32	m_fY;		//!< The Y position.
	FLOAT32 m_fZ;		//!< The Z position (ignored for now).
	UINT32	m_uColour;	//!< The colour of the vertex.
	FLOAT32	m_fTU;		//!< The texture's U coordinate.
	FLOAT32 m_fTV;		//!< The texture's V coordinate.
};

} // end namespace GDE

#endif //_VERTEX_H__Included_1793287761

