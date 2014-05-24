/**
 *	@file MobMoleMadness\EnterTextTool.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _ENTERTEXTTOOL_H__Included_1699345518
#define _ENTERTEXTTOOL_H__Included_1699345518


#include "EditorTool.h"

class CEnterTextListener;

/**
 *	Interface for a tool for getting a text string.
 */
class IEnterTextTool : public IEditorTool
{
public:
	IEnterTextTool( void );
	~IEnterTextTool( void );
	virtual void				Render( void );
	virtual const char*			GetDescription( void ) const = 0;
	virtual const char*			GetName( void ) const = 0;
	virtual bool				RegisterInput( GDE::CInputDevice* pDevice );
	virtual void				ReleaseInput( GDE::CInputDevice* pDevice );

//	void						NotifyLeftClick( void );
//	void						NotifyRightClick( void );
	virtual bool				ProcessCompletion( void ) = 0;
protected:
	const static UINT32			uMAX_FILENAME_LEN = 64;
	CEnterTextListener*			m_pTextEntry;
	char						m_strFileName[uMAX_FILENAME_LEN+1];	// extra one for terminating char.
	const char*					m_strPrompt;
};



#endif //_ENTERTEXTTOOL_H__Included_1699345518

