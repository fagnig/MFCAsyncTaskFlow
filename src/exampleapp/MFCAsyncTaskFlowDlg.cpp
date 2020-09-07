
// MFCAsyncTaskFlowDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCAsyncTaskFlow.h"
#include "MFCAsyncTaskFlowDlg.h"
#include "afxdialogex.h"

#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>

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
}

BEGIN_MESSAGE_MAP(CMFCAsyncTaskFlowDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STARTWORK, &CMFCAsyncTaskFlowDlg::OnBnClickedStartWork1)
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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText(L"Async Taskflow Prototype");

	m_ctrlFileBrowse.EnableFileBrowseButton();

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

void workerThread(const std::filesystem::path& file, const std::string& wordtofind, std::vector<XIAsyncProgressUpdateable*> updateables)
{
	int wordsfound = 0;
	int curline = 0;
	int linecount = 0;

	std::ifstream s;
	s.open(file, std::ifstream::in);

	if( s.is_open() )
	{
		std::string line;
		std::getline(s, line);
		const auto firstlinelen = line.length();
		s.seekg(0);

		const auto fsize = std::filesystem::file_size(file);
		const auto approxlines = fsize/firstlinelen;


		while(!s.eof()) {
			std::string dummy;
			std::getline(s, dummy);
			++linecount;
			if( linecount % 200 == 0 ){
				for( const auto ctrl : updateables ){
					ctrl->UpdateProgress((int)((linecount/(double)approxlines)*30));
				}
			}
		}
		s.clear();
		s.seekg(0);

		while( std::getline(s, line) ){
			++curline;
			size_t pos = 0;
			while( (pos = line.find(wordtofind, pos+1)) != std::string::npos )
			{
				wordsfound++;
			}
			
			if( curline % 200 == 0 ){
				for( const auto ctrl : updateables ){
					ctrl->UpdateProgress(30 + (int)((curline/(double)linecount)*70));
				}
			}
		}
	}

	s.close();

	for( const auto ctrl : updateables )
		ctrl->UpdateResult(wordsfound);
}

void CMFCAsyncTaskFlowDlg::OnBnClickedStartWork1()
{
	std::vector<XIAsyncProgressUpdateable *> updateables;
	updateables.push_back((XIAsyncProgressUpdateable *)&m_ctrlProgressBar);
	updateables.push_back((XIAsyncProgressUpdateable *)&m_buttonStartWork);

	CString filepathbuf;
	m_ctrlFileBrowse.GetWindowText(filepathbuf);
	std::filesystem::path filepath(filepathbuf.GetBuffer());

	CString wordtofindbuf;
	m_editWordToFind.GetWindowText(wordtofindbuf);
	std::string wordtofind = CT2A(wordtofindbuf);

	std::thread worker(workerThread, filepath, wordtofind, updateables);
	worker.detach();
}