#include "pch.h"
#include "CButtonUpdateAble.h"

#include <string>

void CButtonUpdateAble::UpdateProgress(std::any in)
{
  const auto prog = std::any_cast<int>(in);

  EnableWindow( false );

  std::wstring str = L"Working... (";
  str.append(std::to_wstring(prog));
  str.append(L"%)");

  SetWindowText(str.c_str());
}

void CButtonUpdateAble::UpdateResult(std::any in) 
{
  const auto res = std::any_cast<int>(in);

  EnableWindow( true );

  std::wstring str = L"Found ";
  str.append(std::to_wstring(res));
  str.append(L" results.");

  SetWindowText(str.c_str());
}