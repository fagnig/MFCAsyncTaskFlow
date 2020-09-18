#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <any>
#include <thread>
#include "utility/UpdateableContainer.h"

/// <summary>
/// Taks
/// </summary>
class ITask
{
  enum class TaskStatus { NOT_RUN, RUNNING, HALTED, STOPPING, STOPPED, FINISHED };

 public:
  void RunTask(std::string id);

  TaskStatus GetStatus() { return m_status; };

  // Interface part starts here
  virtual void RunTask()  = 0;
  virtual void StopTask() = 0;
  virtual void HaltTask() = 0;

 private:
  std::unordered_map<std::string, std::tuple<std::function<bool(std::vector<std::any>, UpdateableContainer)>, std::vector<std::any>, UpdateableContainer>> m_tasks;


  UpdateableContainer m_updateables;

  TaskStatus m_status{TaskStatus::NOT_RUN};
  bool m_stoptoken{false};

  std::unordered_map<std::string, std::thread> m_taskPool;
};