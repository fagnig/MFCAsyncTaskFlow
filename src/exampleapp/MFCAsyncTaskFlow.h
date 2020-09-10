
// MFCAsyncTaskFlow.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
  #error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"    // main symbols


// CMFCAsyncTaskFlowApp:
// See MFCAsyncTaskFlow.cpp for the implementation of this class
//

class CMFCAsyncTaskFlowApp : public CWinApp
{
public:
  CMFCAsyncTaskFlowApp();

// Overrides
public:
  virtual BOOL InitInstance();

// Implementation

  DECLARE_MESSAGE_MAP()
};

extern CMFCAsyncTaskFlowApp theApp;
