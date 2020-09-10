#pragma once

#include "interface/XIAsyncProgressUpdateable.h"

class CListCtrlUpdateAble : public CListCtrl, XIAsyncProgressUpdateable
{
 public:
  void PreSubclassWindow();

  void UpdateProgress(std::any in) override;

  void UpdateResult(std::any in) override;

private:
  
};

