#include "pch.h"
#include "CButtonThrobberUpdateable.h"

#include <string>

void CButtonThrobberUpdateable::UpdateProgress(std::any in)
{
  const auto prog = std::any_cast<int>(in);

  EnableWindow( false );

  dot = (dot % 3) + 1;

  std::wstring str = L"Working";

  str.append(dot,'.');

  SetWindowText(str.c_str());
}

void CButtonThrobberUpdateable::UpdateResult(std::any in) 
{
  EnableWindow( true );

  dot = 0;

  SetWindowText(L"Start");
}

MAKE_ASYNCUPDATEABLE(CButtonThrobberUpdateable, CButton)