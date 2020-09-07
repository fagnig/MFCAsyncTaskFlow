
// MFCAsyncTaskFlowDlg.h : header file
//

#pragma once

#include "controls/CProgressBarUpdateAble.h"
#include "controls/CButtonUpdateable.h"


// CMFCAsyncTaskFlowDlg dialog
class CMFCAsyncTaskFlowDlg : public CDialogEx
{
// Construction
public:
	CMFCAsyncTaskFlowDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCASYNCTASKFLOW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	CProgressBarUpdateAble	m_ctrlProgressBar;
	CButtonUpdateAble				m_buttonStartWork;
	CMFCEditBrowseCtrl			m_ctrlFileBrowse;
	CEdit										m_editWordToFind;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	//Buttons
	afx_msg void OnBnClickedStartWork1();

	DECLARE_MESSAGE_MAP()
};
