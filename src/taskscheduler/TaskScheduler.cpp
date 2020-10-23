#pragma once

#include "pch.h"
#include "utility/UpdateableContainer.h"
#include "TaskScheduler.h"
#include <thread>


/// @brief Queues a task for being run by the worker threads
/// @param id String identifier to associate task with. To be used for accessing the task later.
/// @param task Shared ptr to an object implementing the ITask interface
void TaskScheduler::EnqueueTask(std::string id, std::shared_ptr<ITask> task)
{
  m_taskQueue.push({id, task});

  m_condition.notify_all();
}

/// @brief Stops a running task
/// @param id String identifier to search the running tasks for.
void TaskScheduler::StopTask(std::string id)
{  
  if( m_runningTasks.find(id) != m_runningTasks.end() )
  {
    m_runningTasks.at(id)->StopTask();
  }
}

/// @brief Suspends a running task
/// @param id String identifier to search the running tasks for.
void TaskScheduler::SuspendTask(std::string id)
{  
  if( m_runningTasks.find(id) != m_runningTasks.end() )
  {
    m_runningTasks.at(id)->SuspendTask();
  }
}

/// @brief Resumes a suspended task
/// @param id String identifier to search the running tasks for.
void TaskScheduler::ResumeTask(std::string id)
{  
  if( m_runningTasks.find(id) != m_runningTasks.end() )
  {
    m_runningTasks.at(id)->ResumeTask();
  }
}

/// @brief Gets the status of a running task
/// @param id String identifier to search the running tasks for.
/// @return The status of the task as a TaskStatus enum, or ITask::TaskStatus::NO_SUCH_TASK if task was not found.
ITask::TaskStatus TaskScheduler::GetTaskStatus(std::string id)
{
  if( m_runningTasks.find(id) != m_runningTasks.end() )
    return m_runningTasks.at(id)->GetStatus();
  else
    return ITask::TaskStatus::NO_SUCH_TASK;
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

      // Waits until there's a job to do, mutex lock makes sure that only one thread grabs a task at a time
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

/// @brief Shuts down the scheduler, should be called before program shutdown.
void TaskScheduler::ShutdownScheduler()
{
  //Kill all idle spinners
  m_killworkers = true;
  m_condition.notify_all();

  //Stop all running tasks, and join their threads
  for( auto & [id, task] : m_runningTasks )
    task->StopTask();

  for( auto & thread : m_threadPool )
    thread.join();

  m_threadPool.clear();

  m_stopped = true;
}