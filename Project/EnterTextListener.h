/**
 *	@file MobMoleMadness\EnterTextListener.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ENTERTEXTLISTENER_H__Included_37134588
#define _ENTERTEXTLISTENER_H__Included_37134588

#include "GDE/Core/Observer.h"

namespace GDE
{
	class CFont;
	class CInputDevice;
};

/**
 *	A listener to allow the user to enter a text string.
 */
class CEnterTextListener : public GDE::IObserver
{
public:
	const static UINT32 uMAX_CHARACTERS_SUPPORTED = 43;

	CEnterTextListener( void );
	~CEnterTextListener( void );
	void	Init( UINT32 uMaxLen );
	void	Render( GDE::CFont* pFont, float fX, float fY, UINT32 uColour, bool bShowCaret );
	char*	GetString( void ) { return m_pString; }
	bool	HasCompleted( void ) const { return m_bHasCompleted; }
	void	Reset( void );

	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage );
	bool	RegisterInput( GDE::CInputDevice* pDevice );
	void	ReleaseInput( GDE::CInputDevice* pDevice );
protected:
	char	GetCharForCode( UINT32 uCode ) const;

	UINT32	m_uMaxStringLen;					//!< Maximum length of string to allow, in characters.
	UINT32	m_uStrLen;							//!< Current length of the string.
	char*	m_pString;							//!< String to be filled.
	bool	m_bHasCompleted;					//!< Has this string entry completed.
	bool	m_bShiftDown;						//!< Is shift down.
	UINT32	m_uShiftCode;						//!< The code for shift.
	UINT32	m_uCodes[uMAX_CHARACTERS_SUPPORTED];//!< Mapping from input code to a char.
};

#endif //_ENTERTEXTLISTENER_H__Included_37134588

