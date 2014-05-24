/**
 *	@file MobMoleMadness\EnterTextListener.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "EnterTextListener.h"

#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"
#include "GDE/Renderer/Font.h"

struct SMapping
{
	char		m_Char;
	char		m_UpperChar;
	const char*	m_strKey;
};

const static SMapping gs_RequiredKeys[] = 
{
	{ '\n','\n', "KeyEnter" },
	{ '\b','\b', "KeyBackspace" },
	{ 'a', 'A', "KeyA" },
	{ 'b', 'B', "KeyB" },
	{ 'c', 'C', "KeyC" },
	{ 'd', 'D', "KeyD" },
	{ 'e', 'E', "KeyE" },
	{ 'f', 'F', "KeyF" },
	{ 'g', 'G', "KeyG" },
	{ 'h', 'H', "KeyH" },
	{ 'i', 'I', "KeyI" },
	{ 'j', 'J', "KeyJ" },
	{ 'k', 'K', "KeyK" },
	{ 'l', 'L', "KeyL" },
	{ 'm', 'M', "KeyM" },
	{ 'n', 'N', "KeyN" },
	{ 'o', 'O', "KeyO" },
	{ 'p', 'P', "KeyP" },
	{ 'q', 'Q', "KeyQ" },
	{ 'r', 'R', "KeyR" },
	{ 's', 'S', "KeyS" },
	{ 't', 'T', "KeyT" },
	{ 'u', 'U', "KeyU" },
	{ 'v', 'V', "KeyV" },
	{ 'w', 'W', "KeyW" },
	{ 'x', 'X', "KeyX" },
	{ 'y', 'Y', "KeyY" },
	{ 'z', 'Z', "KeyZ" },
	{ '0', ')', "Key0" },
	{ '1', '!', "Key1" },
	{ '2', '@', "Key2" },
	{ '3', '#', "Key3" },
	{ '4', '$', "Key4" },
	{ '5', '%', "Key5" },
	{ '6', '^', "Key6" },
	{ '7', '&', "Key7" },
	{ '8', '*', "Key8" },
	{ '9', '(', "Key9" },
	{ ' ', ' ', "KeySpace" },
	{ '-', '-', "KeyMinus" },
	{ '.', '>', "KeyPeriod" },
	{ ',', '<', "KeyComma" },
	{ '/', '?', "KeySlash" },
};

_COMPILE_ASSERT( _ARRAY_SIZE( gs_RequiredKeys ) == CEnterTextListener::uMAX_CHARACTERS_SUPPORTED );

CEnterTextListener::CEnterTextListener( void )
: m_uMaxStringLen( 0 )
, m_uStrLen( 0 )
, m_pString( NULL )
, m_bHasCompleted( false )
{
}

CEnterTextListener::~CEnterTextListener( void )
{
	delete[] m_pString;
}

void CEnterTextListener::Init( UINT32 uMaxLen )
{
	assert_msg( 0 == m_uMaxStringLen, "Cannot init a CEnterTextListener twice" );
	m_uMaxStringLen = uMaxLen;
	m_pString = new char[uMaxLen+2]; // one for the newline, one for the flashing _
	m_pString[0] = 0;
	m_bShiftDown = false;
}

void CEnterTextListener::Reset( void )
{
	m_pString[0] = 0;
	m_uStrLen = 0;
	m_bHasCompleted = false;
}

void CEnterTextListener::Render( GDE::CFont* pFont, float fX, float fY, UINT32 uColour, bool bShowCaret )
{
	if ( m_pString[0] == 0 )
	{
		return;
	}

	const UINT32 colOriginal = pFont->GetColour();
	pFont->SetColour( uColour );
	if ( bShowCaret )
	{
		m_pString[m_uStrLen] = '_';
		m_pString[m_uStrLen+1] = 0;
	}

	pFont->Render( fX, fY, GDE::FJ_Left, m_pString );

	if ( bShowCaret )
	{
		m_pString[m_uStrLen] = 0;
	}
	pFont->SetColour( colOriginal );
}

void CEnterTextListener::NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
{
	UNUSED_VAR( pSubject );

	if ( m_bHasCompleted )
	{
		return; // process no more.
	}

	if ( uMessage == m_uShiftCode )
	{
		GDE::IInputChangeSubject* pChangeSubj = reinterpret_cast<GDE::IInputChangeSubject*>(pSubject);
		if ( &pChangeSubj->GetDevice().GetDownSubject() == pChangeSubj )
		{
			m_bShiftDown = true;
		} else
		{
			m_bShiftDown = false;
		}
		return;
	}

	const char cChar = GetCharForCode( uMessage );
	switch( cChar )
	{
	case '\n':
		m_bHasCompleted = true;
		return;
	case '\b':
		if ( m_uStrLen > 0 )
		{
			m_pString[m_uStrLen--] = 0;
		}
		return;
	default:
		if ( m_uStrLen<m_uMaxStringLen )
		{
			m_pString[m_uStrLen++] = cChar;
		}
	}
	return;
}

bool	CEnterTextListener::RegisterInput( GDE::CInputDevice* pDevice )
{
	for ( UINT32 i=0; i<uMAX_CHARACTERS_SUPPORTED; i++ )
	{
		if ( pDevice->GetInputCode( gs_RequiredKeys[i].m_strKey , m_uCodes[i] ) )
		{
			pDevice->GetDownSubject().AddObserver( this, m_uCodes[i] );
		} else
		{
			DbgPrint( "Unable to bind input for \"%s\"\n", gs_RequiredKeys[i].m_strKey );
			m_uCodes[i] = 0xFFFFFFFF;
		}
	}

	if ( pDevice->GetInputCode( "KeyShift" , m_uShiftCode ) )
	{
		pDevice->GetDownSubject().AddObserver( this, m_uShiftCode );
		pDevice->GetUpSubject().AddObserver( this, m_uShiftCode );
	} else
	{
		m_bShiftDown = false;
		m_uShiftCode = 0xFFFFFFFF;
	}
	 
	return true;
}

void	CEnterTextListener::ReleaseInput( GDE::CInputDevice* pDevice )
{
	for ( UINT32 i=0; i<uMAX_CHARACTERS_SUPPORTED; i++ )
	{
		if ( m_uCodes[i] != 0xFFFFFFFF )
		{
			pDevice->GetDownSubject().RemoveObserver( this, m_uCodes[i] );
		}
	}
	if ( m_uShiftCode != 0xFFFFFFFF )
	{
		pDevice->GetDownSubject().RemoveObserver( this, m_uShiftCode );
		pDevice->GetUpSubject().RemoveObserver( this, m_uShiftCode );
	}
}

char	CEnterTextListener::GetCharForCode( UINT32 uCode ) const
{
	for ( UINT32 i=0; i<uMAX_CHARACTERS_SUPPORTED; i++ )
	{
		if ( m_uCodes[i] == uCode )
		{
			if ( m_bShiftDown )
			{
				return gs_RequiredKeys[i].m_UpperChar;
			} else
			{
				return gs_RequiredKeys[i].m_Char;
			}
		}
	}
	return 0;	// not matched.
}



