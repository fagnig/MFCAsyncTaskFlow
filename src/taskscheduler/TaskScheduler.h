#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <thread>
#include <memory>

#include "utility/UpdateableContainer.h"
#include "interface/ITask.h"

class TaskScheduler
{
 public:
  void AddTask(std::string id, std::shared_ptr<ITask> task);
  void RunTask(std::string id);
  void StopTask(std::string id);
  ITask::TaskStatus GetTaskStatus(std::string id);

 private:
  std::unordered_map<std::string, std::shared_ptr<ITask>> m_tasks;

  std::unordered_map<std::string, std::thread> m_taskPool;
};

static TaskScheduler g_globalTaskManager;