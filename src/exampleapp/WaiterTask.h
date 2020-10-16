
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

  void RunTask()  override;
  void StopTask() override;
  void HaltTask() override;

  int                   m_timetowait;
  UpdateableContainer   m_updateables;
};
