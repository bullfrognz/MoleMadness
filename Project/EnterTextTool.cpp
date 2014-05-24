/**
 *	@file MobMoleMadness\EnterTextTool.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "EnterTextTool.h"
#include "EnterTextListener.h"

//#include "Mouse.h" // TODO: add buttons to browse.

#include "MMMContext.h"
//#include "GDE/Input/InputDevice.h"
//#include "GDE/Input/InputChangeSubject.h"
//#include "GDE/Input/InputStates.h"

#include "GDE/Renderer/Renderer.h"
#include "GDE/Renderer/Vertex.h"
// for untextured:
#include "GDE/Renderer/TextureManager.h"
#include "GDE/Renderer/Texture.h"

#include "GDE/Renderer/Font.h"

extern IEditorTool* gs_EditTool[];

//class IEnterTextToolLeftClickObserver : public GDE::IObserver
//{
//	// for the observer.
//	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
//	{
//		UNUSED_VAR( pSubject );
//		UNUSED_VAR( uMessage );
//		IEnterTextTool::GetInstance().NotifyLeftClick();
//	}
//};

//class IEnterTextToolRightClickObserver : public GDE::IObserver
//{
//	// for the observer.
//	virtual void NotifyChange( GDE::ISubject* pSubject, GDE::UINT32 uMessage )
//	{
//		UNUSED_VAR( pSubject );
//		UNUSED_VAR( uMessage );
//		IEnterTextTool::GetInstance().NotifyRightClick();
//	}
//};

//static IEnterTextToolLeftClickObserver	gs_EnterTextToolLCObserver;
//static CAddWallRightClickObserver	gs_EnterTextToolRCObserver;

IEnterTextTool::IEnterTextTool( void )
{
	m_pTextEntry =  new CEnterTextListener();
	m_pTextEntry->Init( uMAX_FILENAME_LEN );
	m_strPrompt = NULL;

}

IEnterTextTool::~IEnterTextTool( void )
{
	delete m_pTextEntry;
}


bool IEnterTextTool::RegisterInput( GDE::CInputDevice* pDevice )
{
	//GDE::UINT32 uCode;
	//if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_EnterTextToolLCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to bind input for left mouse" );
	//}
	//if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().AddObserver( (GDE::IObserver*)&gs_EnterTextToolRCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to bind input for right mouse" );
	//}
	m_pTextEntry->Reset();
	m_pTextEntry->RegisterInput( pDevice );
	return true;
}

void IEnterTextTool::ReleaseInput( GDE::CInputDevice* pDevice )
{
	m_pTextEntry->ReleaseInput( pDevice );
	//CMMMContext::GetInstance().GetMouse()->SetValidPos( true ); // incase it was false.
	//GDE::UINT32 uCode;
	//if ( pDevice->GetInputCode( "MouseLButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_EnterTextToolLCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to remove input for left mouse" );
	//}
	//if ( pDevice->GetInputCode( "MouseRButton", uCode ) )
	//{
	//	pDevice->GetDownSubject().RemoveObserver( (GDE::IObserver*)&gs_EnterTextToolRCObserver, uCode );
	//} else
	//{
	//	assert_now( "Unable to remove input for right mouse" );
	//}
}

//void IEnterTextTool::NotifyLeftClick( void )
//{
//	EEditorTool eSelected = GetCurrentSelection();
//	if ( eSelected != ETool_COUNT )
//	{
//		CMMMContext::GetInstance().SetEditTool( gs_EditTool[eSelected] );
//	}
//}

//void IEnterTextTool::NotifyRightClick( void )
//{
//}

void IEnterTextTool::Render( void )
{
//	CMMMContext::GetInstance().GetMouse()->SetValidPos( MouseIsValidPos() );
	const float fTOP_LEFT_X = 10.0f;
	const float fTOP_LEFT_Y = 10.0f;
	const float fWIDTH = 780.0f;
	const float fLINE_OFFSET = 20.0f;
	const float fDEPTH = fTOP_LEFT_X + ((NULL!=m_strPrompt)?2:1) * fLINE_OFFSET;

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

	const float fX = fTOP_LEFT_X + 5.0f;
	float fY = fTOP_LEFT_Y + 5.0f;
	if ( m_strPrompt )
	{
		CMMMContext::GetInstance().GetFont()->SetColour( 0xFF7F7FFF );
		CMMMContext::GetInstance().GetFont()->Render( fTOP_LEFT_X + 5.0f, fY, GDE::FJ_Left, m_strPrompt );
		fY += fLINE_OFFSET;
	}

	m_pTextEntry->Render( CMMMContext::GetInstance().GetFont(), fX, fY, 0xFFFFFFFF, true );

	CMMMContext::GetInstance().GetFont()->SetColour( uStartCol );

	// TODO: need an update function
//#pragma warnmsg( "This code should not be in render, instead in update" )
	if ( m_pTextEntry->HasCompleted() )
	{
		if ( ProcessCompletion() )
		{
			CMMMContext::GetInstance().SetEditTool( IEditorTool::GetTool( ETool_SelectTool ) );
		}
	}
}

