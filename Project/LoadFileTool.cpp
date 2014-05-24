/**
 *	@file MobMoleMadness\LoadFileTool.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "LoadFileTool.h"
#include "EnterTextListener.h"

#include "MMMContext.h"

CLoadFileTool::CLoadFileTool( void )
{
	// empty
}

CLoadFileTool::~CLoadFileTool( void )
{
	// emptu
}

CLoadFileTool& CLoadFileTool::GetInstance( void )
{
	static CLoadFileTool Instance;
	return Instance;
}

const char* CLoadFileTool::GetDescription( void ) const
{
	return "Enter a filename to load";
}



bool	CLoadFileTool::ProcessCompletion( void )
{
	return CMMMContext::GetInstance().Load( m_pTextEntry->GetString() );
}

