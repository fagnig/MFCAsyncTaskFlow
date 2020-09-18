#pragma once

#include <any>

class IAsyncProgressUpdateable
{
public:
  virtual void UpdateProgress(std::any in) = 0;

  virtual void UpdateResult(std::any in) = 0;
};