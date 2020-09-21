
// MFCAsyncTaskFlowDlg.h : header file
//

#pragma once

#include "controls/CProgressBarUpdateAble.h"


// CMFCAsyncTaskFlowDlg dialog
class SubDlgTest : public CDialogEx
{
// Construction
public:
  SubDlgTest(CWnd* pParent = nullptr);  // standard constructor

  CProgressBarUpdateAble * GetProgressControlPtr() { return &m_ctrlProgressBar; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_SUBDLGTEST_DIALOG };
#endif

protected:
  virtual void            DoDataExchange(CDataExchange* pDX);  // DDX/DDV support

  CProgressBarUpdateAble  m_ctrlProgressBar;

  virtual BOOL            OnInitDialog() override;

  DECLARE_MESSAGE_MAP()
};
