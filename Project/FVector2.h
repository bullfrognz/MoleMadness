//
//  Media Design School
//  Auckland
//  New Zealand
//
//  File Name   :   FVec2.h
//  Description :   --------------------------
//
//  Copyright © 2004-2010 Media Design School. All Rights Reserved.
//


#pragma once


#ifndef __FVec2_H__
#define __FVec2_H__


// Library Includes
#include "Common.h"


// Local Includes
class CFVec2;
typedef const CFVec2& CFVec2Arg;
typedef CFVec2 SFVec2;


// Types


// Prototypes


class CFVec2
{

	// Member Functions
public:


	 CFVec2();
	 CFVec2(float _fX, float _fY);
	 CFVec2(const CFVec2& _kvrRight);
	~CFVec2();


	float SquareMagnitude() const;
	float Magnitude() const;


	float SquareDistance(const CFVec2& _kvrTarget) const;


	const CFVec2& Normalise();
	const CFVec2& Normalise(float& _frMagnitude);


	const CFVec2& SetPerpendicular();


	CFVec2 GetNormal() const;
	CFVec2 GetNormal(float& _frMagnitude) const;
	bool IsNormal() const;


	float DotProduct(const CFVec2& _kvrArgument) const;
	float DotProductAngle(const CFVec2& _kvrArgument) const;


	void Rotate(float _fRadians);
	const CFVec2& RotateZ(float _fRadians);
	void SetToRotationZ(float _fRadians);


	void SetAbs();


    const CFVec2& operator += (const CFVec2& _kvrRight);
	const CFVec2& operator -= (const CFVec2& _kvrRight);
    const CFVec2& operator *= (float _fValue);
    const CFVec2& operator /= (float _fValue);
	
    CFVec2 operator + (const CFVec2& _kvrRight) const;
    CFVec2 operator - (const CFVec2& _kvrRight) const;
    CFVec2 operator * (float _fValue) const;
    CFVec2 operator / (float _fValue) const;

    bool operator == (const CFVec2& _krRight) const;
    bool operator != (const CFVec2& _krRight) const;


	// Inline Functions
	inline void X(float _fValue);
	inline void Y(float _fValue);


	inline void XAdd(float _fValue);
	inline void YAdd(float _fValue);


	inline float X() const;
	inline float Y() const;


protected:


private:


	// Member Variables
protected:


private:


	float m_fX;
	float m_fY;


};


#include "FVector2.inl"


#endif //__FVec2_H__