#include "pch.h"
#include "CListCtrlUpdateAble.h"

#include <string>

void CListCtrlUpdateAble::PreSubclassWindow()
{
  CListCtrl::PreSubclassWindow();

  CRect rect;
  GetClientRect(&rect);

  ModifyStyle(0,LVS_REPORT);
  InsertColumn(0,L"<blank>"); //Not visible
  SetColumnWidth(0,rect.Width());
  
}

void CListCtrlUpdateAble::UpdateProgress(std::any in)
{
  std::wstring wstr = std::any_cast<std::wstring>(in);
  InsertItem(GetItemCount(), wstr.c_str());
  SetColumnWidth(0,LVSCW_AUTOSIZE);
}

void CListCtrlUpdateAble::UpdateResult(std::any in) 
{
  std::wstring wstr = std::any_cast<std::wstring>(in);
  InsertItem(GetItemCount(), wstr.c_str());
  SetColumnWidth(0,LVSCW_AUTOSIZE);
}