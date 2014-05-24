//
//  Media Design School
//  Auckland
//  New Zealand
//
//  File Name :   Mole.inl
//
//  Copyright © 2004-2010 Media Design School. All Rights Reserved.
//


// Library Includes


// Local Includes


// Implementation


void
CMole::SetHighlighted(bool bHighlighted)
{
	m_bHighlighted = bHighlighted;
}


const CFVec2&
CMole::GetPosition() const
{
	return (m_vPosition);
}


float
CMole::GetRadius() const
{
	return (m_fRadius);
}


float
CMole::GetHeading() const
{
	return (m_fHeading);
}


bool
CMole::IsHighlighted() const
{
	return (m_bHighlighted);
}


CRepulsor*
CMole::GetRepulsor()
{
	return (&m_Repulsor);
}


const CPath*
CMole::GetSharedPath()
{
	return (s_pSharedPath);
}