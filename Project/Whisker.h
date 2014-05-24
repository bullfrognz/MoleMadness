//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  (c) 2011 Bryce Booth
//
//  File Name   :   Whisker.h
//  Description :   --------------------------
//
//  Author      :   Bryce Booth
//  Mail        :   bryce.booth@mediadesign.school.nz
//


#pragma once


#ifndef __Whisker_H__
#define __Whisker_H__


// Library Includes


// Local Includes
#include "FVector2.h"


// Types


// Prototypes


class CWhisker
{

	// Member Functions
public:


	 CWhisker();
	~CWhisker();


	bool Initialise(float _fHeadingOfffset);


	void Update(const CFVec2& _kvrPosition, float _fParentHeading);
	void Draw();


	// Inline Functions
	inline void SetLength(float _fLength);
	inline void SetColour(unsigned int _uiColour);


	inline const CFVec2& GetOriginPoint();
	inline const CFVec2& GetEndPoint();


protected:


private:


	void Deinitialise();


	// Member Variables
protected:


private:


	CFVec2 m_vOriginPoint;
	CFVec2 m_vEndPoint;


	float m_fLength;
	float m_fHeadingOfffset;


	unsigned int m_uiColour;


};


#include "Whisker.inl"


#endif //__Whisker_H__