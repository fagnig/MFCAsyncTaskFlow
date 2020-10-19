
// WaiterTask.h : header file
//

#pragma once

#include "interface/ITask.h"
#include "utility/UpdateableContainer.h"

#include <filesystem>

class WaiterTask : public ITask
{
public:
  WaiterTask(int timetowait ,UpdateableContainer updateables):
    m_timetowait(timetowait), m_updateables(updateables) {};

  void RunTask()    override;
  void OnSuspend()  override;
  void OnResume()   override;
  void OnStopping() override;
  void OnStopped()  override;

  int                   m_timetowait;
  UpdateableContainer   m_updateables;
};
