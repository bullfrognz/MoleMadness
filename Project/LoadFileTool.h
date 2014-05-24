/**
 *	@file MobMoleMadness\LoadFileTool.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _LOADFILETOOL_H__Included_1424385280
#define _LOADFILETOOL_H__Included_1424385280

#include "EnterTextTool.h"

/**
 *	Tool for loading the current level.
 */
class CLoadFileTool : public IEnterTextTool
{
public:
	~CLoadFileTool( void );
	static CLoadFileTool&		GetInstance( void ); // singleton.
	virtual const char*			GetDescription( void ) const;
	virtual const char*			GetName( void ) const { return "Load File"; }

	virtual bool				ProcessCompletion( void );
protected:
	CLoadFileTool( void ); // singleton.
};


#endif //_LOADFILETOOL_H__Included_1424385280

