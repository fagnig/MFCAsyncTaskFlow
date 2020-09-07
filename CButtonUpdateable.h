#pragma once

#include "XIAsyncProgressUpdateable.h"

class CButtonUpdateAble : public CButton, XIAsyncProgressUpdateable
{
public:
  void UpdateProgress(std::any in) override;

  void UpdateResult(std::any in) override;

private:

};

