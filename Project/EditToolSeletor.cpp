/**
 *	@file MobMoleMadness\EditToolSeletor.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "EditToolSelector.h"

#include "Mouse.h"

#include "MMMContext.h"
#include "GDE/Input/InputDevice.h"
#include "GDE/Input/InputChangeSubject.h"
#include "GDE/Input/InputStates.h"

#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/Vertex.h"
// for untextured:
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"

#include "GDE/Renderer/Font.h"

extern IEditorTool* gs_EditTool[];

const float fTOP_LEFT_X = 10.0f;
const float fTOP_LEFT_Y = 10.0f;
const float fWIDTH = 300.0f;
const float fLINE_OFFSET = 20.0f;


class CEditToolSelectorLeftClickObserver : public GDE::IObserver
{
	// for the observer.
	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
	{
		UNUSED_VAR( pSubject );
		UNUSED_VAR( uMessage );
		CEditToolSelector::GetInstance().NotifyLeftClick();
	}
};

//class CEditToolSelectorRightClickObserver : public GDE::IObserver
//{
//	// for the observer.
//	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
//	{
//		UNUSED_VAR( pSubject );
//		UNUSED_VAR( uMessage );
//		CEditToolSelector::GetInstance().NotifyRightClick();
//	}
//};

static CEditToolSelectorLeftClickObserver	gs_EditToolSelectorLCObserver;
//static CAddWallRightClickObserver	gs_EditToolSelectorRCObserver;

CEditToolSelector::CEditToolSelector( void )
{
}

CEditToolSelector& CEditToolSelector::GetInstance( void )
{
	static CEditToolSelector Instance;
	return Instance;
}

const char* CEditToolSelector::GetDescription( void ) const
{
	return "Select the editor tool to use";
}

const char*	CEditToolSelector::GetName( void ) const
{
	return "Select tool";
}

bool CEditToolSelector::RegisterInput( GDE::CInputDevice* pDevice )
{
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_EditToolSelectorLCObserver, uCode );
	} else
	{
		assert_now( "Unable to bind input for left mouse" );
	}
	//if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_EditToolSelectorRCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to bind input for right mouse" );
	//}
	return true;
}

void CEditToolSelector::ReleaseInput( GDE::CInputDevice* pDevice )
{
	CMMMContext::GetInstance().GetMouse()->SetValidPos( true ); // incase it was false.
	GDE::UINT32 uCode;
	if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	{
		pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_EditToolSelectorLCObserver, uCode );
	} else
	{
		assert_now( "Unable to remove input for left mouse" );
	}
	//if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_EditToolSelectorRCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to remove input for right mouse" );
	//}
}

void CEditToolSelector::NotifyLeftClick( void )
{
	EEditorTool eSelected = GetCurrentSelection();
	if ( eSelected != ETool_COUNT )
	{
		CMMMContext::GetInstance().SetEditTool( gs_EditTool[eSelected] );
	}
}

//void CEditToolSelector::NotifyRightClick( void )
//{
//}

void CEditToolSelector::Render( void )
{
//	CMMMContext::GetInstance().GetMouse()->SetValidPos( MouseIsValidPos() );
	const float fTOP_LEFT_X = 10.0f;
	const float fTOP_LEFT_Y = 10.0f;
	const float fWIDTH = 300.0f;
	const float fLINE_OFFSET = 20.0f;

	const float fDEPTH = fTOP_LEFT_X +  (ETool_COUNT-1) * fLINE_OFFSET;

	GDE::SVertex BoxVerts[4];
	BoxVerts[0].m_fX = fTOP_LEFT_X;
	BoxVerts[0].m_fY = fTOP_LEFT_Y;
	BoxVerts[1].m_fX = fTOP_LEFT_X+fWIDTH;
	BoxVerts[1].m_fY = fTOP_LEFT_Y;
	BoxVerts[2].m_fX = fTOP_LEFT_X+fWIDTH;
	BoxVerts[2].m_fY = fTOP_LEFT_Y+fDEPTH;
	BoxVerts[3].m_fX = fTOP_LEFT_X;
	BoxVerts[3].m_fY = fTOP_LEFT_Y+fDEPTH;

	for ( UINT32 i=0; i<4; i++ )
	{
		BoxVerts[i].m_fZ = 0.5f;
		BoxVerts[i].m_uColour = 0x3F000000;	// very grey
	}

	// untextured:
	CMMMContext::GetInstance().GetRenderer()->GetTextureManager().GetNULLTexture()->Apply( 0 );
	CMMMContext::GetInstance().GetRenderer()->DrawPrimitives( GDE::PT_QuadList, 1, BoxVerts );

	const UINT32 uStartCol = CMMMContext::GetInstance().GetFont()->GetColour();

	const EEditorTool eCurrent = GetCurrentSelection();

	float fY = fTOP_LEFT_Y + 5.0f;
	for ( UINT32 i=1; i<ETool_COUNT; i++ )
	{
		const UINT32 uCol = (i==(UINT32)eCurrent)?0xFF7F7FFF:0xFFFFFFFF;
		CMMMContext::GetInstance().GetFont()->SetColour( uCol );
		CMMMContext::GetInstance().GetFont()->Render( fTOP_LEFT_X + 5.0f, fY, GDE::FJ_Left, gs_EditTool[i]->GetName() );
		fY += fLINE_OFFSET;
	}
	CMMMContext::GetInstance().GetFont()->SetColour( uStartCol );
}

EEditorTool CEditToolSelector::GetCurrentSelection( void ) const
{
	const CFVec2& vPos = CMMMContext::GetInstance().GetMouse()->GetPosition();
	if ( vPos.X() < fTOP_LEFT_X )
	{
		return ETool_COUNT;
	}
	if ( vPos.X() > fTOP_LEFT_X + fWIDTH )
	{
		return ETool_COUNT;
	}
	if ( vPos.Y() < fTOP_LEFT_Y )
	{
		return ETool_COUNT;
	}
	float fY = fTOP_LEFT_Y + fLINE_OFFSET;
	for ( UINT32 i=1; i<ETool_COUNT; i++ )
	{
		if ( vPos.Y() < fY )
		{
			return EEditorTool( i );
		}
		fY += fLINE_OFFSET;
	}
	return ETool_COUNT;
}
