#pragma once

#include "pch.h"
#include "utility/UpdateableContainer.h"
#include "TaskScheduler.h"
#include <thread>

void TaskScheduler::AddTask(std::string id, std::function<bool(std::vector<std::any>, UpdateableContainer)> functor, std::vector<std::any> args, UpdateableContainer updateables)
{
  m_tasks.insert({id, {functor,args, updateables}});
}

void TaskScheduler::RunTask(std::string id)
{
  auto [functor, args, updateables] = m_tasks.at(id);

  std::thread worker(functor, args, updateables);
  worker.detach();
}