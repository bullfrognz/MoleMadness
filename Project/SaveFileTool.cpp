/**
 *	@file MobMoleMadness\SaveFileTool.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "SaveFileTool.h"
#include "EnterTextListener.h"

#include "MMMContext.h"

CSaveFileTool::CSaveFileTool( void )
{
	// empty
}

CSaveFileTool::~CSaveFileTool( void )
{
	// emptu
}

CSaveFileTool& CSaveFileTool::GetInstance( void )
{
	static CSaveFileTool Instance;
	return Instance;
}

const char* CSaveFileTool::GetDescription( void ) const
{
	return "Enter a filename to save as";
}



bool	CSaveFileTool::ProcessCompletion( void )
{
	return CMMMContext::GetInstance().Save( m_pTextEntry->GetString() );
}

