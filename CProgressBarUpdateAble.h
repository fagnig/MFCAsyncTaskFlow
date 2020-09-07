#pragma once

#include "XIAsyncProgressUpdateable.h"

class CProgressBarUpdateAble : public CProgressCtrl, XIAsyncProgressUpdateable
{
 public:

  void UpdateProgress(std::any in) override;

  void UpdateResult(std::any in) override;

private:
  
};

