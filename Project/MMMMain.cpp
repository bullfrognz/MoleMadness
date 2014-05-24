/**
 *	@file MobMoleMadness\MMMMain.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/

#include "Common.h"
#include "MMMContext.h"

#include "GDE/Renderer/Renderer.h"
#include "GDE/Input/InputDevice.h"
#include "GDE/DataParser/DataParserManager.h"
#include "GDE/Core/DLLLoader.h"
#include "GDE/Renderer/FontManager.h"
#include "GDE/Renderer/SpriteManager.h"

#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


#ifdef _DEBUG
const char* g_strDllName = "HGERenderer_D.dll";
#else
const char* g_strDllName = "HGERenderer.dll";
#endif

/**
 *	Simply initialise and kick off the game (application) loop..
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{	
	GDE::CRenderer TheRenderer;
	if ( false == TheRenderer.LoadDLL( g_strDllName ) )
	{
		printf( "Unable to load %s\n", g_strDllName );
		assert_now( "Unable to load %s\n", g_strDllName );
		return 0;
	}
	GDE::CInputDevice TheInput;
	if ( false == TheInput.LoadDLL( g_strDllName ) )
	{
		printf( "Unable to load input from %s\n", g_strDllName );
		assert_now( "Unable to load input from %s\n", g_strDllName );
		return 1;
	}

	GDE::CDataParserManager DataManager;
	if ( false == DataManager.LoadDLL( g_strDllName ) )
	{
		assert_now( "Error loading parsing dll \"%s\"", g_strDllName );
		return -1;
	}

	GDE::CFontManager FontMgr;
	if ( false == FontMgr.LoadDLL( g_strDllName, &TheRenderer ) )
	{
		assert_now( "Error loading font dll \"%s\"", g_strDllName );
		return -1;	
	}

	GDE::CSpriteManager SpriteMan;
	if ( false == SpriteMan.LoadDLL( g_strDllName, &TheRenderer ) )
	{
		assert_now( "Error loading sprite dll \"%s\"", g_strDllName );
		return -1;	
	}

	if ( false == CMMMContext::GetInstance().Initialise(
			TheRenderer, 
			TheInput, 
			FontMgr,
			DataManager,
			SpriteMan ) )
	{
		assert_now( "Error initialising the MobMoleMadness app" );
		return -1;
	}

	TheRenderer.RunGameLoop();
	CMMMContext::GetInstance().ReleaseResources();
	return 0;
}

