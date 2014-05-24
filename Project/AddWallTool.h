/**
 *	@file MobMoleMadness\AddWallTool.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ADDWALLTOOL_H__Included_999235994
#define _ADDWALLTOOL_H__Included_999235994

#include "EditorTool.h"
#include "FVector2.h"

/**
 *	Tool for adding walls.
 */
class CAddWallTool : public IEditorTool
{
public:
	static CAddWallTool&		GetInstance( void ); // singleton.
	virtual void		Render( void );
	virtual const char* GetDescription( void ) const;
	virtual const char* GetName( void ) const { return "Add Walls"; }
	virtual bool		RegisterInput( GDE::CInputDevice* pDevice );
	virtual void		ReleaseInput( GDE::CInputDevice* pDevice );

	void				NotifyLeftClick( void );
	void				NotifyRightClick( void );
protected:
	CAddWallTool( void ); // singleton.
	const static UINT32 uMAX_SEGMENTS = 128;
	UINT32	m_uNumPoints;
	CFVec2	m_vSegmentEnds[uMAX_SEGMENTS*2];
};

#endif //_ADDWALLTOOL_H__Included_999235994

