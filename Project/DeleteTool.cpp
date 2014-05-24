/**
 *	@file MobMoleMadness\DeleteTool.cpp
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "DeleteTool.h"
#include "Box2D.h"

#include "MMMContext.h"

CDeleteTool::CDeleteTool( void )
{
	// empty
}

CDeleteTool::~CDeleteTool( void )
{
	// emptu
}

CDeleteTool& CDeleteTool::GetInstance( void )
{
	static CDeleteTool Instance;
	return Instance;
}

const char* CDeleteTool::GetDescription( void ) const
{
	return "Click to drag and select a region to delete items from";
}

bool	CDeleteTool::SelectionComplete( const CFVec2& vSecondCorner )
{
	UNUSED_VAR( vSecondCorner );
	CMMMContext::GetInstance().DeleteHighlighedItems();
	return false;
}

