#include "pch.h"
#include "CButtonThrobberUpdateable.h"

#include <string>

BEGIN_MESSAGE_MAP(CButtonThrobberUpdateable, CButton)
  ON_WM_TIMER()
  MAKE_ASYNCUPDATEABLE_MSGMAP(CButtonThrobberUpdateable)
END_MESSAGE_MAP()

MAKE_ASYNCUPDATEABLE_FUNCS(CButtonThrobberUpdateable)

void CButtonThrobberUpdateable::UpdateProgress(std::any in)
{
  SetTimer(1, 500, nullptr);

  EnableWindow( false );

}

void CButtonThrobberUpdateable::UpdateResult(std::any in) 
{
  EnableWindow( true );

  dot = 0;
  
  KillTimer(1);

  SetWindowText(L"Start");
}

void CButtonThrobberUpdateable::OnTimer(UINT nIDEvent)
{
  SetTimer(1, 500, nullptr);

  dot = (dot % 3) + 1;
  std::wstring str = L"Working";
  str.append(dot,'.');

  SetWindowText(str.c_str());
}