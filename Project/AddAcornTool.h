/**
 *	@file MobMoleMadness\AddAcornTool.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ADDACORNTOOL_H__Included_629984804
#define _ADDACORNTOOL_H__Included_629984804

#include "EditorTool.h"

/**
 *	Tool for adding acorns.
 */
class CAddAcornTool : public IEditorTool
{
public:
	static CAddAcornTool&	GetInstance( void ); // singleton.
	virtual void			Render( void );
	virtual const char*		GetDescription( void ) const;
	virtual const char*		GetName( void ) const { return "Add Acorns"; }
	virtual bool			RegisterInput( GDE::CInputDevice* pDevice );
	virtual void			ReleaseInput( GDE::CInputDevice* pDevice );

	void					NotifyLeftClick( void );
protected:
	bool					MouseIsValidPos( void ) const;
	CAddAcornTool( void ); // singleton.
};

#endif //_ADDACORNTOOL_H__Included_629984804

