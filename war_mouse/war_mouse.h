
// war_mouse.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cwar_mouseApp:
// See war_mouse.cpp for the implementation of this class
//

class Cwar_mouseApp : public CWinApp
{
public:
	Cwar_mouseApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cwar_mouseApp theApp;