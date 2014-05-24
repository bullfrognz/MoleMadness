/**
 *	@file MobMoleMadness\SelectBoxTool.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _SELECTBOXTOOL_H__Included_894470704
#define _SELECTBOXTOOL_H__Included_894470704

#include "EditorTool.h"
#include "FVector2.h"

class CSelectBoxLeftClickObserver;

/**
 *	Tool for selecting box regions of items.
 */
class ISelectBoxTool : public IEditorTool
{
public:
	ISelectBoxTool( void );
	virtual ~ISelectBoxTool();
	virtual void			Render( void );
	virtual const char*		GetDescription( void ) const = 0;
	virtual const char*		GetName( void ) const = 0;
	virtual bool			RegisterInput( GDE::CInputDevice* pDevice );
	virtual void			ReleaseInput( GDE::CInputDevice* pDevice );

	void					NotifyLeftClickDown( void );
	void					NotifyLeftClickUp( void );
	virtual bool			SelectionComplete( const CFVec2& vSecondCorner ) = 0;
protected:

	bool					m_bSelectionActive;
	CFVec2					m_vFirstCorner;
	CSelectBoxLeftClickObserver* m_pObserver;
};


#endif //_SELECTBOXTOOL_H__Included_894470704

