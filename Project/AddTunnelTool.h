/**
 *	@file MobMoleMadness\AddTunnelTool.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ADDTUNNELTOOL_H__Included_912759961
#define _ADDTUNNELTOOL_H__Included_912759961

#include "EditorTool.h"

/**
 *	Tool for adding tunnels.
 */
class CAddTunnelTool : public IEditorTool
{
public:
	static CAddTunnelTool&	GetInstance( void ); // singleton.
	virtual void			Render( void );
	virtual const char*		GetDescription( void ) const;
	virtual const char*		GetName( void ) const { return "Add Tunnels"; }
	virtual bool			RegisterInput( GDE::CInputDevice* pDevice );
	virtual void			ReleaseInput( GDE::CInputDevice* pDevice );

	void					NotifyLeftClick( void );
protected:
	bool					MouseIsValidPos( void ) const;
	CAddTunnelTool( void ); // singleton.
};
#endif //_ADDTUNNELTOOL_H__Included_912759961

