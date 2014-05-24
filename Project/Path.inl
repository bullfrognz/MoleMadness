//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//
//  File Name :   Path.inl
//
//  Author    :   Bryce Booth
//  Mail      :   bryce.booth@mediadesign.school.nz
//


// Library Includes


// Local Includes


// Implementation

#ifndef _EDITOR
const Path::TNode&
CPath::GetNode(int _iIndex) const
{
	return (m_tpNodes[_iIndex]);
}


int
CPath::GenerateNodeId(int _iIndexX, int _iIndexY)
{
	return (_iIndexY * m_iNumNodesX + _iIndexX);
}


int
CPath::GenerateNodeId(const Path::TNode& _ktrNode)
{
	return (GenerateNodeId(_ktrNode.iX, _ktrNode.iY));
}

bool
CPath::HasUndiscoveredNodesLeft() const
{
	return (m_bHasUndiscoveredNodes);
}


#endif