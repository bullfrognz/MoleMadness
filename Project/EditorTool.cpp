/**
 *	@file MobMoleMadness\EditorTool.cpp
 *	@brief Get at each editor tool instance.
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#include "Common.h"
#include "EditorTool.h"

#include "EditToolSelector.h"
#include "AddWallTool.h"
#include "AddAcornTool.h"
#include "AddMoleTool.h"
#include "SaveFileTool.h"
#include "LoadFileTool.h"
#include "AddTunnelTool.h"
#include "DeleteTool.h"

IEditorTool* gs_EditTool[] = 
{
	&CEditToolSelector::GetInstance(),
	&CAddWallTool::GetInstance(),
	&CAddTunnelTool::GetInstance(),
	&CAddAcornTool::GetInstance(),
	&CAddMoleTool::GetInstance(),
	&CDeleteTool::GetInstance(),
	&CLoadFileTool::GetInstance(),
	&CSaveFileTool::GetInstance(),
};

_COMPILE_ASSERT( _ARRAY_SIZE( gs_EditTool ) == ETool_COUNT );

IEditorTool*		IEditorTool::GetTool( EEditorTool eTool )
{
	return gs_EditTool[ eTool ];
}
