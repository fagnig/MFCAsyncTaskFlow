
// MFCAsyncTaskFlowDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SubDlgTest.h"
#include "afxdialogex.h"

#include <resource.h>

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

SubDlgTest::SubDlgTest(CWnd* pParent /*=nullptr*/)
  : CDialogEx(IDD_SUBDLGTEST_DIALOG, pParent)
{
  Create(IDD_SUBDLGTEST_DIALOG, pParent);
}

void SubDlgTest::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgressBar);
}

BEGIN_MESSAGE_MAP(SubDlgTest, CDialogEx)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMFCAsyncTaskFlowDlg message handlers

BOOL SubDlgTest::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  SetWindowText(L"External Window Progressbar");

  return TRUE;  // return TRUE  unless you set the focus to a control
}