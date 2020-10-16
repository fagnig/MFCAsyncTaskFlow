
// MFCAsyncTaskFlowDlg.h : header file
//

#pragma once

#include "controls/CProgressBarUpdateAble.h"
#include "controls/CButtonUpdateable.h"
#include "controls/CButtonThrobberUpdateable.h"
#include "controls/CListCtrlUpdateAble.h"
#include "SubDlgTest.h"

#include <memory>

// CMFCAsyncTaskFlowDlg dialog
class CMFCAsyncTaskFlowDlg : public CDialogEx
{
// Construction
public:
  CMFCAsyncTaskFlowDlg(CWnd* pParent = nullptr);  // standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_MFCASYNCTASKFLOW_DIALOG };
#endif

protected:
  virtual void            DoDataExchange(CDataExchange* pDX);  // DDX/DDV support

  // Member variables
  HICON m_hIcon;

  CProgressBarUpdateAble    m_ctrlProgressBar;
  CButtonUpdateAble         m_buttonStartWork;
  CMFCEditBrowseCtrl        m_ctrlFileBrowse;
  CEdit                     m_editWordToFind;
  CListCtrlUpdateAble       m_listLog;

  CEdit                     m_editSecondsToThink;
  CButtonThrobberUpdateable m_buttonStartWait;

  std::unique_ptr<SubDlgTest> m_subdlg;

  virtual BOOL            OnInitDialog() override;

  // Message map functions
  afx_msg void            OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void            OnPaint();
  afx_msg HCURSOR         OnQueryDragIcon();
  afx_msg void            OnGetMinMaxInfo(MINMAXINFO * lpMMI);
  
  // Message map - Buttons
  afx_msg void            OnBnClickedStartWork();
  afx_msg void            OnBnClickedStartWait();

  DECLARE_MESSAGE_MAP()

private:
  int32_t m_iSizeMinX{300};
  int32_t m_iSizeMinY{300};

};
