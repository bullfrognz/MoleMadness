/**
 *	@file GDE\GDE\Renderer\RendererContext.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _RENDERERCONTEXT_H__Included_686728766
#define _RENDERERCONTEXT_H__Included_686728766

#include "Common.h"

namespace GDE
{

/**
 *	The renderer context is used to initialize the renderer so it can,
 *	for example, create the correct size window.
 *
 *	Any application that needs to use the renderer has to subclass this
 *	and implement the FrameUpdate function (@see FrameUpdate).
 *
 *	@author Jon Booth.
 */
class IRendererContext
{
public:
	virtual ~IRendererContext( void ){}; // cause all deconstuctors to be virtual
	IRendererContext( void );

	/**
	 *	This is run to update the frame
	 *	@return true if the application is to continue running, false if it is to quit.
	 */
	virtual bool			FrameUpdate( void ) = 0;
	virtual UINT32			GetDisplayWidth( void ) const;
	virtual UINT32			GetDisplayHeight( void ) const;
	virtual UINT32			GetDisplayDepth( void ) const;
	virtual bool			IsWindowed( void ) const;
	virtual const char*		GetWindowTitle( void ) const;
protected:
	UINT32		m_uDispWidth;		//!< Width of the display in pixels.
	UINT32		m_uDispHeight;		//!< Height of the display in pixels.
	UINT32		m_uDispDepth;		//!< Depth of the display in bits.
	bool		m_bWindowed;		//!< Is the display windowed?
	const char*	m_strWindowTitle;	//!< The window/application title to be used.
};

};

#endif //_RENDERERCONTEXT_H__Included_686728766

