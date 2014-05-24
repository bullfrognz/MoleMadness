/**
 *	@file GDE\GDE\Renderer\FontImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _FONTIMP_H__Included_1898586659
#define _FONTIMP_H__Included_1898586659

#include "GDE/Core/Resource.h"
#include "GDE/Renderer/FontConstants.h"

namespace GDE
{

class CRenderer;

/**
 *	The implementation of the font class.
 */
class IFontImp : public IResource
{
public:
	/**
	 *	Draw the message at the specified place.
	 *	Unknown characters are drawn as '?'.
	 *	Line breaks ('\n') should be handled properly.
	 *
	 *	@param fX			Screen X coordinate.
	 *	@param fY			Screen Y coordinate.
	 *	@param eJustify		The type of justification to use.
	 *	@param strMessage	The message to be drawn on the screen.
	 */
	virtual bool Render( FLOAT32 fX, FLOAT32 fY, EFontJustification eJustify, const char* strMessage ) = 0;

	/**
	 *	@param colFont	The colour to set the font to for subsequent draw operations.
	 */
	virtual bool SetColour( UINT32 colFont ) = 0;

	/**
	 *	@return The current colour of the font.
	 */
	virtual UINT32 GetColour( void ) const = 0;

	/**
	 *	@return The height of the font, in pixels.
	 */
	virtual FLOAT32 GetHeight( void ) const = 0;

	/**
	 *	@return The width of the string, in pixels.
	 */
	virtual FLOAT32 GetStringWidth( const char* strMessage ) const = 0;
};

} // end namespace GDE

#endif //_FONTIMP_H__Included_1898586659

