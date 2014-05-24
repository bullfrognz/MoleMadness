/**
 *	@file GDE\GDE\Core\FVector3.h
 *	@brief 
 *	
 *	Copyright()
 *		Copyright © 2004-2010 Media Design School. All Rights Reserved.
 *	
 *********************************************************************************************/
#ifndef _FVECTOR3_H__Included_1478343780
#define _FVECTOR3_H__Included_1478343780

namespace GDE
{

/**
 *	A three component floating point vector.
 */
class CFVec3
{
public:
	static CFVec3 Init( FLOAT32 fX, FLOAT32 fY, FLOAT32 fZ = 0.0f )
	{
		CFVec3 vRV;
		vRV.SetX( fX );
		vRV.SetY( fY );
		vRV.SetZ( fZ );
		return vRV;
	}
	FLOAT32 GetX( void ) const { return m_fX; }
	FLOAT32 GetY( void ) const { return m_fY; }
	FLOAT32 GetZ( void ) const { return m_fZ; }
	void	SetX( FLOAT32 fX ) { m_fX = fX; }
	void	SetY( FLOAT32 fY ) { m_fY = fY; }
	void	SetZ( FLOAT32 fZ ) { m_fZ = fZ; }
protected:
	FLOAT32 m_fX;
	FLOAT32	m_fY;
	FLOAT32 m_fZ;
};

static const CFVec3 gs_vZERO = CFVec3::Init( 0.0f, 0.0f, 0.0f );

} // end namespace GDE

#endif //_FVECTOR3_H__Included_1478343780

