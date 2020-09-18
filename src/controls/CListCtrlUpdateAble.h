#pragma once

#include "interface/IAsyncProgressUpdateable.h"

class CListCtrlUpdateAble : public CListCtrl, IAsyncProgressUpdateable
{
 public:
  void PreSubclassWindow();

  void UpdateProgress(std::any in) override;

  void UpdateResult(std::any in) override;

private:
  
};

