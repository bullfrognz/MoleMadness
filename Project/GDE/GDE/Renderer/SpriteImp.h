/**
 *	@file GDE\GDE\Renderer\SpriteImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SPRITEIMP_H__Included_702853277
#define _SPRITEIMP_H__Included_702853277

#include "GDE/Core/Resource.h"

namespace GDE
{

class CRenderer;
class CSprite;
struct SFRect;
class CTexture;

/**
 *	The implementation of the sprite class.
 */
class ISpriteImp : public IResource
{
public:
	/**
	 *	Render the sprite with the hotspot at the given coordinates.
	 *	@see SetHotSpot.
	 *	@param fX	The screen X coordinate to render at.
	 *	@param fY	The screen Y coordinate to render at.
	 *
	 *	@return Success.
	 */
	virtual bool Render( FLOAT32 fX, FLOAT32 fY ) = 0;

	/**
	 *	Render the sprite with the hotspot at the given coordinates.
	 *	@see SetHotSpot.
	 *	@param fX		The screen X coordinate to render at.
	 *	@param fY		The screen Y coordinate to render at.
	 *	@param fRot		Clockwise rotation in radians around the hotspot.
	 *	@param fXScale	Horizontal scaling to be applied.
	 *	@param fYScale	Vertical scaling to be applied.
	 *
	 *	@return Success.
	 */
	virtual bool RenderEx( FLOAT32 fX, FLOAT32 fY, FLOAT32 fRot, FLOAT32 fXScale, FLOAT32 fYScale ) = 0;

	/**
	 *	@param colSprite	The colour to set the sprite tint too.
	 *	@return Success.
	 */
	virtual bool SetColour( UINT32 colSprite ) = 0;

	/**
	 *	@return The current colour of the sprite tint or zero if not implemented.
	 */
	virtual UINT32 GetColour( void ) const = 0;

	/**
	 *	@param The texture to set for the sprite page.
	 *	@return Success.
	 */
	virtual bool SetTexture( CTexture* pTexture ) = 0;

	/**
	 *	@return The current sprite texture or NULL if not implemented.
	 */
	virtual CTexture* GetTexture( void ) const = 0;

	/**
	 *	Set the texture rectangle for the sprite to use from the current texture page.
	 *
	 *	@param Rect	The rectangle to use.
	 *	@return Success.
	 */
	virtual bool SetTextureRect( const SFRect& Rect ) = 0;

	/**
	 *	Get the texture rectangle the sprite uses from the current texture page.
	 *
	 *	@param Rect	Filled with the rectangle in use.
	 *	@return Success.
	 */
	virtual bool GetTextureRect( SFRect& Rect ) const = 0;

	/**
	 *	Set the hot spot offset for the sprite.
	 *	@param fX The horizontal offset into the texture rect.
	 *	@param fY The vertical offset into the texture rect.
	 *	@return Success.
	 */
	virtual bool SetHotSpot( FLOAT32 fX, FLOAT32 fY ) = 0;
	/**
	 *	Get the hot spot offset in use for the sprite.
	 *	@param fX Filled with the horizontal offset into the texture rect.
	 *	@param fY Filled with the vertical offset into the texture rect.
	 *	@return Success.
	 */
	virtual bool GetHotSpot( FLOAT32& fX, FLOAT32& fY ) const = 0;

	/**
	 *	@return The width of the sprite in pixels or zero if not implemented.
	 */
	virtual FLOAT32 GetWidth( void ) const = 0;

	/**
	 *	@return The height of the sprite in pixels or zero if not implemented.
	 */
	virtual FLOAT32 GetHeight( void ) const = 0;

};

} // end namespace GDE

#endif //_SPRITEIMP_H__Included_702853277

