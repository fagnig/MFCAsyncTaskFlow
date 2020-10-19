
// MFCAsyncTaskFlowDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCAsyncTaskFlow.h"
#include "MFCAsyncTaskFlowDlg.h"
#include "afxdialogex.h"
#include "utility/UpdateableContainer.h"
#include "taskscheduler/TaskScheduler.h"
#include "WordCounterTask.h"
#include "WaiterTask.h"
#include "SubDlgTest.h"

#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>

#include "fmt/core.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std::chrono_literals;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
  CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_ABOUTBOX };
#endif

  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCAsyncTaskFlowDlg dialog



CMFCAsyncTaskFlowDlg::CMFCAsyncTaskFlowDlg(CWnd* pParent /*=nullptr*/)
  : CDialogEx(IDD_MFCASYNCTASKFLOW_DIALOG, pParent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAsyncTaskFlowDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgressBar);
  DDX_Control(pDX, IDC_STARTWORK, m_buttonStartWork);
  DDX_Control(pDX, IDC_FILEBROWSE, m_ctrlFileBrowse);
  DDX_Control(pDX, IDC_WORDTOFIND, m_editWordToFind);
  DDX_Control(pDX, IDC_LOGLIST, m_listLog);

  DDX_Control(pDX, IDC_SECONDSTOTHINK, m_editSecondsToThink);
  DDX_Control(pDX, IDC_STARTWORK2, m_buttonStartWait);
}

BEGIN_MESSAGE_MAP(CMFCAsyncTaskFlowDlg, CDialogEx)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_WM_GETMINMAXINFO()
  ON_BN_CLICKED(IDC_STARTWORK, &CMFCAsyncTaskFlowDlg::OnBnClickedStartWork)
  ON_BN_CLICKED(IDC_STARTWORK2, &CMFCAsyncTaskFlowDlg::OnBnClickedStartWait)
END_MESSAGE_MAP()


// CMFCAsyncTaskFlowDlg message handlers

BOOL CMFCAsyncTaskFlowDlg::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if( pSysMenu != nullptr )
  {
    BOOL bNameValid;
    CString strAboutMenu;
    bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
    ASSERT(bNameValid);
    if( !strAboutMenu.IsEmpty() )
    {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);      // Set big icon
  SetIcon(m_hIcon, FALSE);    // Set small icon

  //Set minimum size of the window
  CRect rect;
  GetWindowRect(&rect);
  m_iSizeMinX = rect.Width();
  m_iSizeMinY = rect.Height();

  SetWindowText(L"Async Task Prototype");

  m_ctrlFileBrowse.EnableFileBrowseButton();

  m_editSecondsToThink.SetWindowText(L"3");

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCAsyncTaskFlowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if( (nID & 0xFFF0) == IDM_ABOUTBOX )
  {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  }
  else
  {
    CDialogEx::OnSysCommand(nID, lParam);
  }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCAsyncTaskFlowDlg::OnPaint()
{
  if( IsIconic() )
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialogEx::OnPaint();
  }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCAsyncTaskFlowDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

void CMFCAsyncTaskFlowDlg::OnBnClickedStartWork()
{
  switch( g_globalTaskManager.GetTaskStatus("worker") )
  {
    case ITask::TaskStatus::RUNNING:
      g_globalTaskManager.SuspendTask("worker");
      return;
    case ITask::TaskStatus::SUSPENDED:
      g_globalTaskManager.ResumeTask("worker");
      return;
  }

  m_listLog.DeleteAllItems();

  UpdateableContainer updateables;
  updateables.AddProgressUpdateTarget("progbar", "numeric", m_ctrlProgressBar.GetSafeHwnd() );
  updateables.AddProgressUpdateTarget("strtbtn", "numeric", m_buttonStartWork.GetSafeHwnd() );
  updateables.AddProgressUpdateTarget("listlog", "textual", m_listLog.GetSafeHwnd() );

  if( !m_subdlg )
    m_subdlg = std::make_unique<SubDlgTest>(this);

  updateables.AddProgressUpdateTarget("progbarext", "numeric", m_subdlg->GetProgressControlPtr()->GetSafeHwnd() );

  m_subdlg->ShowWindow(SW_SHOW);

  CRect rc;
  GetWindowRect(&rc);
  m_subdlg->SetWindowPos(&wndTop, rc.right, rc.top, 0, 0, SWP_NOSIZE);

  CString filepathbuf;
  m_ctrlFileBrowse.GetWindowText(filepathbuf);
  std::filesystem::path filepath(filepathbuf.GetBuffer());

  CString wordtofindbuf;
  m_editWordToFind.GetWindowText(wordtofindbuf);
  std::string wordtofind = CT2A(wordtofindbuf);

  auto taskptr = std::make_shared<WordCounterTask>(wordtofind, filepath, updateables);
  g_globalTaskManager.AddTask("worker", taskptr);
  
  std::string str = fmt::format("Started searching '{}' for the word '{}'.", filepath.string(), wordtofind);
  std::wstring wstr = ATL::CA2W(str.c_str());
  m_listLog.UpdateProgress(wstr);
}

void CMFCAsyncTaskFlowDlg::OnGetMinMaxInfo(MINMAXINFO * lpMMI)
{
  lpMMI->ptMinTrackSize.x = m_iSizeMinX;
  lpMMI->ptMinTrackSize.y = m_iSizeMinY;
}

void CMFCAsyncTaskFlowDlg::OnBnClickedStartWait()
{
  UpdateableContainer updateables;
  updateables.AddProgressUpdateTarget("throbberbutton", "throbber", m_buttonStartWait.GetSafeHwnd() );
  updateables.AddProgressUpdateTarget("listlog", "textual", m_listLog.GetSafeHwnd() );

  CString waittimebuf;
  m_editSecondsToThink.GetWindowText(waittimebuf);
  int waittime = _ttoi(waittimebuf);

  auto taskptr = std::make_shared<WaiterTask>(waittime,updateables);
  g_globalTaskManager.AddTask("waiter", taskptr);

  m_buttonStartWait.UpdateProgress(1);

  std::string str = fmt::format("Asynchronously waiting for {} seconds.", waittime);
  std::wstring wstr = ATL::CA2W(str.c_str());
  m_listLog.UpdateProgress(wstr);
}
