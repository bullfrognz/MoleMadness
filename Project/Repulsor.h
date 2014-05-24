//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  File Name   :   Repulsor.h
//  Description :   --------------------------
//
//  Author      :   Bryce Booth
//  Mail        :   bryce.booth@mediadesign.school.nz
//


#pragma once


#ifndef __Repulsor_H__
#define __Repulsor_H__


// Library Includes


// Local Includes
#include "FVector2.h"
#include "Circle.h"


// Types


// Prototypes


class CRepulsor
{

	// Member Functions
public:


	 CRepulsor() {};
	~CRepulsor() {};



	void Initialise( const CFVec2& _vrPosition, float _fRadius, float _fRepulsonRadius, float _fMultiplier = 1.0f)
	{
		m_vPosition = _vrPosition;
		m_fRadius = _fRadius;
		m_fRepulsonRadius = _fRepulsonRadius;
		m_fMultiplier = _fMultiplier;
	}


	void Draw()
	{
		CCircle Circle;
		Circle.Initialise(m_vPosition, m_fRepulsonRadius + m_fRadius);
		Circle.Draw(0x3FFF00FF);
	}


	CFVec2& GetPosition() { return (m_vPosition); }
	float GetRadius() { return (m_fRadius); }
	float GetRepulsionRadius() { return (m_fRepulsonRadius); }
	float GetMultiplier() { return (m_fMultiplier); }


	// Inline Functions


protected:


private:


	// Member Variables
protected:


private:


	CFVec2 m_vPosition;
	float m_fRadius;
	float m_fRepulsonRadius;

	float m_fMultiplier;


};


#endif //__Repulsor_H__