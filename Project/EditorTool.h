/**
 *	@file MobMoleMadness\EditorTool.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _EDITORTOOL_H__Included_1725500628
#define _EDITORTOOL_H__Included_1725500628

namespace GDE
{
	class CInputDevice;
}

enum EEditorTool
{
	// THE SELECT TOOL MUST COME FIRST
	ETool_SelectTool,	//!< Tool to select the next tool from a menu.
	ETool_DrawWall,		//!< Tool for drawing a wall.
	ETool_PlaceTunnel,	//!< Placing some tunnels.
	ETool_PlaceAcorn,	//!< Placing some acorns.
	ETool_PlaceMole,	//!< Placing a mole.
	ETool_Delete,		//!< Deleting objects.
	ETool_LoadFile,		//!< Load a file.
	ETool_SaveFile,		//!< Saving a file.
	ETool_COUNT			//!< Must come last.
};

/**
 *	Abstract interface each editor tool must switch between.
 */
class IEditorTool
{
public:
	virtual ~IEditorTool( void ) {};
	virtual void			Render( void ) = 0;
	virtual const char*		GetDescription( void ) const = 0;
	virtual const char*		GetName( void ) const = 0;
	virtual bool			RegisterInput( GDE::CInputDevice* pDevice ) = 0;
	virtual void			ReleaseInput( GDE::CInputDevice* pDevice ) = 0;

	static IEditorTool*		GetTool( EEditorTool eTool );
};



#endif //_EDITORTOOL_H__Included_1725500628

