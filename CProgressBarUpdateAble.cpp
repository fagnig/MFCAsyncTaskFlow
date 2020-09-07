#include "pch.h"
#include "CProgressBarUpdateAble.h"

void CProgressBarUpdateAble::UpdateProgress(std::any in)
{
  SetBarColor(CLR_DEFAULT);
  const auto prog = std::any_cast<int>(in);
  SetPos(prog);
}

void CProgressBarUpdateAble::UpdateResult(std::any in) 
{
  SetPos(100);

  SetBarColor(RGB(255,255,0));
}