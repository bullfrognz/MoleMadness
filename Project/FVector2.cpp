//
//  Media Design School
//  Auckland
//  New Zealand
//
//  File Name :   FVector2.cpp
//
//  Copyright © 2004-2010 Media Design School. All Rights Reserved.
//


// Local Includes
#include "Common.h"
#include "FVector2.h"


// Library Includes
#include <math.h>


// This Include
#include "FVector2.h"


// Static Variables


// Implementation


/********************************
            Public
*********************************/





/*---------------------------------------------------------------------------------------------------------------------------
*
* FVec2 constructors
*
*---------------------------------------------------------------------------------------------------------------------------*/

CFVec2::CFVec2()
: m_fX(0)
, m_fY(0)
{
	//Empty
}





CFVec2::CFVec2(float _fX, float _fY)
: m_fX(_fX)
, m_fY(_fY)
{
	//Empty
}





CFVec2::CFVec2(const CFVec2& _kvrRight)
: m_fX(_kvrRight.X())
, m_fY(_kvrRight.Y())
{
	//Empty
}





/*---------------------------------------------------------------------------------------------------------------------------
*
* FVec2 deconstructor
*
*---------------------------------------------------------------------------------------------------------------------------*/

CFVec2::~CFVec2()
{
	// Empty
}




float
CFVec2::SquareMagnitude() const
{
	return (X()*X()+Y()*Y());
}





float
CFVec2::Magnitude() const
{
	return sqrtf( SquareMagnitude() );
}





float
CFVec2::SquareDistance(const CFVec2& _kvrTarget) const
{
	return ((*this) - _kvrTarget ).SquareMagnitude();
}





const CFVec2&
CFVec2::Normalise()
{
	(*this) /= ( Magnitude() );
	return (*this);

	/*
	assert( this != NULL );
	const float Mag = Magnitude();
	assert( Mag != 0.0f );

	const float fMult = 1.0f/Mag;

	X( X() * fMult );
	Y( Y() * fMult );
	*/


	//return (*this);
}





const CFVec2&
CFVec2::Normalise(float& _frMagnitude)
{
	assert( this != NULL );
	_frMagnitude = Magnitude();
	assert( _frMagnitude != 0.0f );


	const float fMult = 1.0f/_frMagnitude;


	X( X() * fMult );
	Y( Y() * fMult );


	return (*this);
}





const CFVec2&
CFVec2::SetPerpendicular()
{
	const float& t = X();
	X( Y() );
	Y( -t );


	return (*this);
}





CFVec2
CFVec2::GetNormal() const
{
	CFVec2 RetVal = *this;
	return RetVal.Normalise();
}





CFVec2
CFVec2::GetNormal(float& _frMagnitude) const
{
	CFVec2 RetVal = *this;


	return RetVal.Normalise(_frMagnitude);
}





bool
CFVec2::IsNormal() const
{
	return(fabsf(SquareMagnitude() - 1.0f) <= 0.0001f);
}





float
CFVec2::DotProduct(const CFVec2& _kvrArgument) const
{
	float RetVal = (X() * _kvrArgument.X()) + (Y() * _kvrArgument.Y());
	return RetVal;
}





float
CFVec2::DotProductAngle(const CFVec2& _kvrArgument) const
{
	float fUV = (X() * _kvrArgument.X()) + (Y() * _kvrArgument.Y());


	float fUMag = this->Magnitude();
	float fVMag = _kvrArgument.Magnitude();


	float fAngle = acos(fUV / (fUMag * fVMag));


	return (fAngle);
}





void
CFVec2::Rotate(float _fRadians)
{
	float fNewX = cosf(_fRadians) * X() - sinf(_fRadians) * Y();
	float fNewY = sinf(_fRadians) * X() + cosf(_fRadians) * Y();


	X(fNewX);
	Y(fNewY);
}





/*---------------------------------------------------------------------------------------------------------------------------
*
*	@param fRadians Amount to rotate clockwise, in radians.
*
*---------------------------------------------------------------------------------------------------------------------------*/

const CFVec2&
CFVec2::RotateZ(float _fRadians)
{
	const float fSin = sinf(-_fRadians);
	const float fCos = cosf(-_fRadians);
	const float fX = fCos*m_fX + fSin*m_fY;


	m_fY = -fSin*m_fX + fCos*m_fY;
	m_fX = fX;


	return (*this);
}






/*---------------------------------------------------------------------------------------------------------------------------
*
*	Set this vector to the clockwise rotation (from straight up) around Z.
*
*	@param fRadians Amount to rotate clockwise, in radians from straight up.
*
*---------------------------------------------------------------------------------------------------------------------------*/

void
CFVec2::SetToRotationZ(float _fRadians)
{
	const float fSin = sinf(fPI - _fRadians);
	const float fCos = cosf(fPI - _fRadians);


	m_fX = fSin;
	m_fY = fCos;
}





/**
 *	Set both X and Y to their absolute values.
 */
void
CFVec2::SetAbs()
{
	m_fX = fabsf( m_fX );
	m_fY = fabsf( m_fY );
}





const CFVec2&
CFVec2::operator += (const CFVec2& _kvrRight)
{
	m_fX += _kvrRight.X();
	m_fY += _kvrRight.Y();


	return (*this);
}





const CFVec2&
CFVec2::operator -= (const CFVec2& _kvrRight)
{
	m_fX -= _kvrRight.X();
	m_fY -= _kvrRight.Y();


	return (*this);
}





const CFVec2&
CFVec2::operator *= (float _fValue)
{
	m_fX *= _fValue;
	m_fY *= _fValue;


	return (*this);
}





const CFVec2&
CFVec2::operator /= (float _fValue)
{
	m_fX /= _fValue;
	m_fY /= _fValue;


	return (*this);
}





CFVec2 
CFVec2::operator + (const CFVec2& _kvrRight) const
{
	CFVec2 vResult(m_fX + _kvrRight.X(),
				   m_fY + _kvrRight.Y());


	return (vResult);
}





CFVec2
CFVec2::operator - (const CFVec2& _kvrRight) const
{
	CFVec2 vResult(m_fX - _kvrRight.X(),
				   m_fY - _kvrRight.Y());


	return (vResult);
}





CFVec2
CFVec2::operator * (float _fValue) const
{
	CFVec2 vResult(m_fX * _fValue,
				   m_fY * _fValue);


	return (vResult);
}





CFVec2
CFVec2::operator / (float _fValue) const
{
	CFVec2 vResult(m_fX / _fValue,
				   m_fY / _fValue);


	return (vResult);
}





bool
CFVec2::operator == (const CFVec2&_kvrRight) const
{
	if (m_fX == _kvrRight.X() &&
		m_fY == _kvrRight.Y())
	{
		return (true);
	}
	else
	{
		return (false);
	}
}





bool
CFVec2::operator != (const CFVec2&_kvrRight) const
{
	if (m_fX != _kvrRight.X() ||
		m_fY != _kvrRight.Y())
	{
		return (true);
	}
	else
	{
		return (false);
	}
}





/********************************
            Protected
*********************************/






/********************************
            Private
*********************************/
