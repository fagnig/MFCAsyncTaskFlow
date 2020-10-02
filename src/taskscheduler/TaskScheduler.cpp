#pragma once

#include "pch.h"
#include "utility/UpdateableContainer.h"
#include "TaskScheduler.h"
#include <thread>

void TaskScheduler::AddTask(std::string id, std::shared_ptr<ITask> task)
{
  m_taskQueue.push({id, task});

  m_condition.notify_all();
}

void TaskScheduler::RunTask(std::string id)
{
  if( m_runningTasks.find(id) != m_runningTasks.end() )
  {
    auto task = m_runningTasks.at(id);

    std::thread worker(&ITask::RunTask, task.get());
    worker.detach();
  }
}

void TaskScheduler::StopTask(std::string id)
{  
  if( m_runningTasks.find(id) != m_runningTasks.end() )
  {
    m_runningTasks.at(id)->StopTask();
  }
}

ITask::TaskStatus TaskScheduler::GetTaskStatus(std::string id)
{
  if( m_runningTasks.find(id) != m_runningTasks.end() )
    return m_runningTasks.at(id)->GetStatus();
  else
    return ITask::TaskStatus::NOT_RUN;
}


TaskScheduler::TaskScheduler()
{
  const int nmaxthread = std::thread::hardware_concurrency() - 1; // Leave a thread open for UI
  
  for(int i = 0; i < nmaxthread; i++)
    m_threadPool.push_back(std::thread(&TaskScheduler::TaskSpinner, this));

}

TaskScheduler::~TaskScheduler()
{
  if( !m_stopped ) ShutdownScheduler();
}

void TaskScheduler::TaskSpinner()
{
  for( ;; )
  {
    std::shared_ptr<ITask>  ptr;
    std::string             taskid;
    {
      std::unique_lock<std::mutex> lock(m_queuelock);

      // Waits until there's a job to do
      m_condition.wait(lock, [this](){ return !m_taskQueue.empty() || m_killworkers; });
      if( m_killworkers ) return;

      auto [id, task] = m_taskQueue.front();
      m_taskQueue.pop();

      // Insert it into the map so we can reference it later
      m_runningTasks.insert({id, task});

      ptr = task;
      taskid = id;
    }

    ptr->RunTask();
    // Clean up the task reference when the task has finished
    m_runningTasks.erase(taskid);
  }
}

void TaskScheduler::ShutdownScheduler()
{
  m_killworkers = true;

  m_condition.notify_all();

  for( auto & [id, task] : m_runningTasks )
    task->StopTask();

  for( auto & thread : m_threadPool )
    thread.join();

  m_threadPool.clear();

  m_stopped = true;
}