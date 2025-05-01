#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTblEditorApp:
// See TblEditor.cpp for the implementation of this class
//

class CTblEditorApp : public CWinApp
{
public:
	CTblEditorApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTblEditorApp theApp;
