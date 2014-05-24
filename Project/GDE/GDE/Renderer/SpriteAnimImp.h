/**
 *	@file GDE\GDE\Renderer\SpriteAnimImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SPRITEANIMIMP_H__Included_652602311
#define _SPRITEANIMIMP_H__Included_652602311

#include "GDE/Core/Resource.h"

namespace GDE
{

class CRenderer;
class CSprite;
struct SFRect;
class CTexture;

/**
 *	The implementation of the sprite animation class.
 *	@TODO: make an animation controller abstraction that can play many different animating things?
 */
class ISpriteAnimImp : public IResource
{
public:
	/**
	 *	Play or resume playing an animation.
	 *
	 *	@return Success.
	 */
	virtual bool	Play( void ) = 0;
	/**
	 *	@return Is the animation playing, or false on failure.
	 */
	virtual bool	IsPlaying( void ) const = 0;
	/**
	 *	Stop, or pause a playing animation.
	 *
 	 *	@return Success.
	 */
	virtual bool	Stop( void ) = 0;
	/**
	 *	Reset the animation to the start.
	 *
 	 *	@return Success.
	 */
	virtual bool	Reset( void ) = 0;
	/**
	 *	Set the animation to looping.
	 *
 	 *	@return Success.
	 */
	virtual bool	SetLooping( bool bValue ) = 0;
	/**
	 *	@return Is the animation looping, or false on failure.
	 *
	 *	@return The previous looping state or false on error.
	 */
	virtual bool	IsLooping( void ) const = 0;
	/**
	 *	Set the animation to playing backwards.
	 *	@return The previous state or false on error.
	 */
	virtual bool	SetReverse( bool bValue ) = 0;
	/**
	 *	@return Is the animation playing in reverse, or false on failure.
	 */
	virtual bool	IsReverse( void ) const = 0;
	/**
	 *	Set the animation to ping pong - play once forwards and then reverse back through.
	 *	@return the previous state or false on error.
	 */
	virtual bool	SetPingPong( bool bValue ) = 0;
	/**
	 *	@return Is the animation doing ping-ping cycles, or false on failure.
	 */
	virtual bool	IsPingPong( void ) const = 0;
	/**
	 *	Set the current animation playback speed, 1.0f for normal operation.
	 *
 	 *	@return Success.
	 */
	virtual bool	SetSpeed( GDE::FLOAT32 fSpeed ) = 0;
	/**
	 *	@return The current animation playback speed, 1.0f for normal operation.
	 */
	virtual GDE::FLOAT32	GetSpeed( void ) const = 0;
	/**
	 *	@return the current frame number.
	 */
	virtual GDE::UINT32	GetFrame( void ) const = 0;
	/**
	 *	@return the number of frames.
	 */
	virtual GDE::UINT32	GetFrameCount( void ) const = 0;
	/**
	 *	Update the animation
	 *	@param fTimeDelta The time difference to update over.
	 *
 	 *	@return Success.
	 */
	virtual bool	Update( GDE::FLOAT32 fTimeDelta ) = 0;
	/**
	 *	@return The sprite this animation modifies, or NULL on failure.
	 */
	virtual CSprite* GetSprite( void ) = 0;
};

} // end namespace GDE

#endif //_SPRITEANIMIMP_H__Included_652602311

