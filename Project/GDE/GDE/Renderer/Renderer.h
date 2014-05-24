/**
 *	@file GDE\GDE\Renderer\Renderer.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _RENDERER_H__Included_921652092
#define _RENDERER_H__Included_921652092

//#include "GDE\Core\Core.h"
#include "GDE\Core\Bridge.h"
#include "GDE\Renderer\Primitives.h"

namespace GDE
{

class IRendererImp;
class IRendererContext;
struct SVertex;
class CTextureManager;
class CFontManager;

class CRenderer : public TBridge< IRendererImp, CREATE_DLL_ID( 'R','N','D','R') >
{
public:
	typedef TBridge<IRendererImp,CREATE_DLL_ID( 'R','N','D','R')> TSuperClass; //!< Convenience typedef to the super class.

	// TODO: should this be a singleton?
	CRenderer( void );
	~CRenderer( void );

	// these functions are for initialisation:
	bool SetContext( IRendererContext& Context );
	void RunGameLoop( void );

	// these functions are for asset management:

	// these functions are for rendering:
	bool BeginScene( void );
	bool EndScene( void );
	bool ColourClear( UINT32 colClear );

	bool DrawPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, const void* pVertices, UINT32 uVertexStride );
	template< typename TPrimitive > bool DrawPrimitives( EPrimitiveType eType, UINT32 uPrimitiveCount, TPrimitive* pVertices )
	{
		return DrawPrimitives( eType, uPrimitiveCount, pVertices, sizeof(TPrimitive) );
	}

	CTextureManager&	GetTextureManager( void );
protected:
	CTextureManager*	m_pTextureManager;	//!< The texture manager associated with this renderer.
};

} // end namespace GDE

#endif //_RENDERER_H__Included_921652092

