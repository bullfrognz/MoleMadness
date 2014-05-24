/**
 *	@file GDE\GDE\DataParser\DataParserImp.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _DATAPARSERIMP_H__Included_1995467253
#define _DATAPARSERIMP_H__Included_1995467253

#include "GDE/Core/Resource.h"

namespace GDE
{

class CRenderer;

/**
 *	The implementation of the Data Parser class.
 */
class IDataParserImp : public IResource
{
public:
	/**
	 *	@param uValue		Value to be filled in - left alone on fail.
	 *	@param strSection	Section name to lookup in.
	 *	@param strId		Id to be looked up..
	 *	@return				Success.
	 */
	virtual bool	GetValue( UINT32& uValue, const char* strSection, const char* strId ) = 0;
	/**
	 *	@param uValue		Value to set.
	 *	@param strSection	Section name to set in.
	 *	@param strId		Id to be set.
	 *	@return				Success.
	 */
	virtual bool	SetValue( UINT32 uValue, const char* strSection, const char* strId ) = 0;
	/**
	 *	@param iValue		Value to be filled in - left alone on fail.
	 *	@param strSection	Section name to lookup in.
	 *	@param strId		Id to be looked up..
	 *	@return				Success.
	 */
	virtual bool	GetValue( SINT32& iValue, const char* strSection, const char* strId ) = 0;
	/**
	 *	@param iValue		Value to set.
	 *	@param strSection	Section name to set in.
	 *	@param strId		Id to be set.
	 *	@return				Success.
	 */
	virtual bool	SetValue( SINT32 iValue, const char* strSection, const char* strId ) = 0;
	/**
	 *	@param fValue		Value to be filled in - left alone on fail.
	 *	@param strSection	Section name to lookup in.
	 *	@param strId		Id to be looked up..
	 *	@return				Success.
	 */
	virtual bool	GetValue( FLOAT32& fValue, const char* strSection, const char* strId ) = 0;
	/**
	 *	@param fValue		Value to set.
	 *	@param strSection	Section name to set in.
	 *	@param strId		Id to be set.
	 *	@return				Success.
	 */
	virtual bool	SetValue( FLOAT32 fValue, const char* strSection, const char* strId ) = 0;

	/**
	 *	@param strValue		Value to be filled in - left alone on fail, unless uMaxChars is met.
	 *	@param uMaxChars	The maximum number of characters to fill in. Will return fail if exceeded.
	 *	@param strSection	Section name to lookup in.
	 *	@param strId		Id to be looked up..
	 *	@return				Success.
	 */
	virtual bool	GetString( char* strValue, const UINT32 uMaxChars, const char* strSection, const char* strId ) = 0;
	/**
	 *	@param strValue		Value to set.
	 *	@param strSection	Section name to set in.
	 *	@param strId		Id to be set.
	 *	@return				Success.
	 */
	virtual bool	SetString( const char* strValue, const char* strSection, const char* strId ) = 0;
};

} // end namespace GDE

#endif //_DATAPARSERIMP_H__Included_1995467253

