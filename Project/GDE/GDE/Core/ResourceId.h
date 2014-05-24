/**
 *	@file GDE\GDE\Core\ResourceId.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _RESOURCEID_H__Included_2058410576
#define _RESOURCEID_H__Included_2058410576

namespace GDE
{
	class CResourceId
	{
	public:
		CResourceId( const char* strFileName );
		CResourceId( const CResourceId& Other );
		~CResourceId( void );
		bool operator == (const CResourceId& Other ) const;
		const char* GetFileName( void ) const;
	protected:
		void Init( const char* strFileName  );
		// TODO: add other, unique id properties.
//		const char* m_strId;			//!< A unique identification string.
		const char* m_strFileName;		//!< The filename.
	};

}// end namespace GDE

#endif //_RESOURCEID_H__Included_2058410576

