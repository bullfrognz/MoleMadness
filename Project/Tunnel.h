/**
 *	@file MobMoleMadness\Tunnel.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _TUNNEL_H__Included_1774015416
#define _TUNNEL_H__Included_1774015416

#include "FVector2.h"

namespace GDE
{
	class CSprite;
};

class CCircle;
class CBox2D;

/**
 *	The tunnels, entry and exit points for the squirrel.
 */
class CTunnel
{
public:
	~CTunnel( void );
	bool		Init( const CFVec2& vPosition, const char* strTexture );
	void		Draw( void );
	bool		Intersects( const CCircle& Circle ) const;
	bool		Intersects( const CBox2D& Box ) const;
	const CFVec2&	GetPosition( void ) const { return m_vPosition; }

	void SetHighlighted( bool bHighlighted ) { m_bHighlighted = bHighlighted; }
	bool IsHighlighted( void ) const { return m_bHighlighted; }

protected:
	CFVec2				m_vPosition;		//!< The position of the acorn.
	GDE::CSprite*		m_pSprite;			//!< The sprite used to draw.
	bool				m_bHighlighted;		//!< Is this highlighted?
};


#endif //_TUNNEL_H__Included_1774015416

