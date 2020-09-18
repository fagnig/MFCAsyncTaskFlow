#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <any>
#include <thread>
#include "utility/UpdateableContainer.h"

class TaskScheduler
{
 public:
  void AddTask(std::string id, std::function<bool(std::vector<std::any>, UpdateableContainer)> functor, std::vector<std::any> args, UpdateableContainer updateables);
  void RunTask(std::string id);

 private:
  std::unordered_map<std::string, std::tuple<std::function<bool(std::vector<std::any>, UpdateableContainer)>, std::vector<std::any>, UpdateableContainer>> m_tasks;

  std::unordered_map<std::string, std::thread> m_taskPool;
};

static TaskScheduler g_globalTaskManager;