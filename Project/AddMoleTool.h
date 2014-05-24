/**
 *	@file MobMoleMadness\AddMoleTool.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ADDMOLETOOL_H__Included_50609897
#define _ADDMOLETOOL_H__Included_50609897

#include "EditorTool.h"
#include "FVector2.h"

/**
 *	Tool for adding Moles.
 */
class CAddMoleTool : public IEditorTool
{
public:
	static CAddMoleTool&	GetInstance( void ); // singleton.
	virtual void			Render( void );
	virtual const char*		GetDescription( void ) const;
	virtual const char*		GetName( void ) const { return "Add Moles"; }
	virtual bool			RegisterInput( GDE::CInputDevice* pDevice );
	virtual void			ReleaseInput( GDE::CInputDevice* pDevice );

	void					NotifyLeftClick( void );
//	void					NotifyRightClick( void );
protected:
	bool					MouseIsValidPos( void ) const;
	CAddMoleTool( void ); // singleton.
	CFVec2					m_vMolePos;
	bool					m_bGetRotation;
};

#endif //_ADDMOLETOOL_H__Included_50609897

