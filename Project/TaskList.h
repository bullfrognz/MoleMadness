//
//  Bryce Booth
//  Auckland
//  New Zealand
//
//  (c) 2011 Bryce Booth
//
//  File Name   :   TaskList.h
//  Description :   --------------------------
//
//  Author      :   Bryce Booth
//  Mail        :   bryce.booth@mediadesign.school.nz
//


#pragma once


#ifndef _EDITOR
#ifndef __TaskList_H__
#define __TaskList_H__


// Library Includes
#include <stack>


// Local Includes
#include "Task.h"


// Types


// Prototypes


class CTaskList
{

	// Member Types
public:

	// Member Functions
public:


	 CTaskList();
	~CTaskList();


	bool Initialise();


	// Inline Functions


protected:


private:


	void Deinitialise();


	// Member Variables
protected:


private:


};


#include "TaskList.inl"


#endif //__TaskList_H__
#endif //_EDITOR