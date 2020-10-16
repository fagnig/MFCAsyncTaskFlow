#pragma once

#include "interface/IAsyncProgressUpdateable.h"

class CButtonThrobberUpdateable : public CButton, IAsyncProgressUpdateable
{
public:
  void UpdateProgress(std::any in) override;

  void UpdateResult(std::any in) override;

  DECLARE_MESSAGE_MAP()
  MAKE_ASYNCUPDATEABLE_HEADER_NOMSGMAP()


  afx_msg void OnTimer(UINT nIDEvent);
private:

  int dot;
};

