//
//  Media Design School
//  Auckland
//  New Zealand
//
//  File Name :   FVector2.inl
//
//  Copyright © 2004-2010 Media Design School. All Rights Reserved.
//


// Library Includes


// Local Includes


// Implementation


void
CFVec2::X(float _fValue)
{
	m_fX = _fValue;
}


void
CFVec2::Y(float _fValue)
{
	m_fY = _fValue;
}


void
CFVec2::XAdd(float _fValue)
{
	m_fX += _fValue;
}


void
CFVec2::YAdd(float _fValue)
{
	m_fY += _fValue;
}


float
CFVec2::X() const
{
	return (m_fX);
}


float
CFVec2::Y() const
{
	return (m_fY);
}