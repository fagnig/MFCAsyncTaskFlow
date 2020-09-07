
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
	HICON m_hIcon;

	CProgressBarUpdateAble	m_ctrlProgressBar;
	CButtonUpdateAble				m_buttonStartWork;
	CMFCEditBrowseCtrl			m_ctrlFileBrowse;
	CEdit										m_editWordToFind;
	CListCtrl								m_listLog;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO * lpMMI);

	//Buttons
	afx_msg void OnBnClickedStartWork();

	DECLARE_MESSAGE_MAP()

private:
	int32_t m_iSizeMinX{300};
	int32_t m_iSizeMinY{300};
};
