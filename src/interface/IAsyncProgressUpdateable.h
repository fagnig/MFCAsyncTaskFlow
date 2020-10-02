#pragma once

#include <any>

/// <summary>
/// Interface for allowing a UI control to be updated asynchronously.
/// UpdateProgress can called as many times as one would like. 
/// UpdateResult should be called once, ideally at the end of the task.
/// See CProgressBarUpdateAble for an example of an implementation of this interface.
/// 
/// Technically one could have this update anything, not necessarily a UI Control.
/// </summary>
class IAsyncProgressUpdateable
{
public:
  virtual void UpdateProgress(std::any in) = 0;

  virtual void UpdateResult(std::any in) = 0;
};