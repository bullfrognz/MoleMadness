/**
 *	@file MobMoleMadness\Acorn.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ACORN_H__Included_445248611
#define _ACORN_H__Included_445248611


#include "FVector2.h"

namespace GDE
{
	class CSprite;
};

class CCircle;
class CBox2D;
class CMole;

/**
 *	The acorn or treasure the squirrel wants to gain.
 */
class CAcorn
{
public:
	enum EState
	{
		ES_Available = 0,	//!< This acorn is available for collection
		ES_Carried,			//!< This acorn is currently carried
		ES_Collected		//!< This acorn has been collected.
	};
	CAcorn( void );
	~CAcorn( void );
	bool		Init( const CFVec2& vPosition, const char* strTexture );
	void		Draw( void );
	bool		Intersects( const CCircle& Circle ) const;
	bool		Intersects( const CBox2D& Box ) const;
	const CFVec2&	GetPosition( void ) const { return m_vPosition; }

	void		SetHighlighted( bool bHighlighted ) { m_bHighlighted = bHighlighted; }
	bool		IsHighlighted( void ) const { return m_bHighlighted; }
	EState		GetState( void ) const { return m_eState; }
	void		SetState(EState eState);


	void Update(CMole* _pMoles, int _iNumMoles);

	void SetMole(CMole* _pMole) { m_pOwnerMole = _pMole; }
	CMole* GetMole() { return (m_pOwnerMole); }


protected:
	CFVec2				m_vPosition;		//!< The position of the acorn.
	GDE::CSprite*		m_pSprite;			//!< The sprite used to draw.
	bool				m_bHighlighted;		//!< Is this highlighted?
	EState				m_eState;			//!< The current state of the acorn.
	CMole* m_pOwnerMole;


	bool m_bBeenFound;
};
#endif //_ACORN_H__Included_445248611

