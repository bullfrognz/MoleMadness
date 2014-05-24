/**
 *	@file MobMoleMadness\EditToolSelector.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _EDITTOOLSELECTOR_H__Included_932342288
#define _EDITTOOLSELECTOR_H__Included_932342288


#include "EditorTool.h"
#include "FVector2.h"

/**
 *	Tool for selecting the next editor tool.
 */
class CEditToolSelector : public IEditorTool
{
public:
	static CEditToolSelector&	GetInstance( void ); // singleton.
	virtual void				Render( void );
	virtual const char*			GetDescription( void ) const;
	virtual const char*			GetName( void ) const;
	virtual bool				RegisterInput( GDE::CInputDevice* pDevice );
	virtual void				ReleaseInput( GDE::CInputDevice* pDevice );

	void						NotifyLeftClick( void );
//	void						NotifyRightClick( void );
protected:
	CEditToolSelector( void ); // singleton.
	EEditorTool					GetCurrentSelection( void ) const;
};

#endif //_EDITTOOLSELECTOR_H__Included_932342288

