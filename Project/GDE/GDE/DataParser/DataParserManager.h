/**
 *	@file GDE\GDE\DataParser\DataParserManager.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _DATAPARSERMANAGER_H__Included_1867569826
#define _DATAPARSERMANAGER_H__Included_1867569826


#include "GDE/Core/ResourceManagerBridge.h"

namespace GDE
{

	class CRenderer;
	class IDataParserManagerImp;
	class CDataParser;

	class CDataParserManager : public TResourceManagerBridge< CDataParserManager, IDataParserManagerImp, CDataParser, CREATE_DLL_ID( 'D','P','R','S') >
	{
	public:
		typedef TResourceManagerBridge< CDataParserManager, IDataParserManagerImp, CDataParser, CREATE_DLL_ID( 'D','P','R','S') > TParent;

		CDataParserManager( void )
			: TParent()
		{	
		}
		// Does nothing except default manager behaviour.
	protected:
	};
}	// end namespace GDE




#endif //_DATAPARSERMANAGER_H__Included_1867569826

