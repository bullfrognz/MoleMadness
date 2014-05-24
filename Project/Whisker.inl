//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//
//  File Name :   Whisker.inl
//
//  Author    :   Bryce Booth
//  Mail      :   bryce.booth@mediadesign.school.nz
//


// Library Includes


// Local Includes


// Implementation


void
CWhisker::SetLength(float _fLength)
{
	m_fLength = _fLength;
}


void
CWhisker::SetColour(unsigned int _uiColour)
{
	m_uiColour = _uiColour;
}


const CFVec2&
CWhisker::GetOriginPoint()
{
	return (m_vOriginPoint);
}


const CFVec2&
CWhisker::GetEndPoint()
{
	return (m_vEndPoint);
}