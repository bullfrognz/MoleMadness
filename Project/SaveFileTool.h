/**
 *	@file MobMoleMadness\SaveFileTool.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SAVEFILETOOL_H__Included_1820973342
#define _SAVEFILETOOL_H__Included_1820973342

#include "EnterTextTool.h"

/**
 *	Tool for saving the current file.
 */
class CSaveFileTool : public IEnterTextTool
{
public:
	~CSaveFileTool( void );
	static CSaveFileTool&		GetInstance( void ); // singleton.
	virtual const char*			GetDescription( void ) const;
	virtual const char*			GetName( void ) const { return "Save File"; }

	virtual bool				ProcessCompletion( void );
protected:
	CSaveFileTool( void ); // singleton.
};

#endif //_SAVEFILETOOL_H__Included_1820973342

