#pragma once

#include "interface/IAsyncProgressUpdateable.h"

class CProgressBarUpdateAble : public CProgressCtrl, IAsyncProgressUpdateable
{
 public:

  void UpdateProgress(std::any in) override;

  void UpdateResult(std::any in) override;

  MAKE_ASYNCUPDATEABLE_HEADER()

private:
  
};