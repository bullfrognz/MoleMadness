/**
 *	@file MobMoleMadness\DeleteTool.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _DELETETOOL_H__Included_1462601581
#define _DELETETOOL_H__Included_1462601581

#include "SelectBoxTool.h"

class CDeleteTool : public ISelectBoxTool
{
public:
	~CDeleteTool( void );
	static CDeleteTool&		GetInstance( void ); // singleton.
	virtual const char*		GetDescription( void ) const;
	virtual const char*		GetName( void ) const { return "Delete Items"; }

	virtual bool			SelectionComplete( const CFVec2& vSecondCorner );
protected:
	CDeleteTool( void ); // singleton.
};

#endif //_DELETETOOL_H__Included_1462601581

