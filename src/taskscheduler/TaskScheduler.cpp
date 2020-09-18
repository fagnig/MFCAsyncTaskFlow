#pragma once

#include "pch.h"
#include "utility/UpdateableContainer.h"
#include "TaskScheduler.h"
#include <thread>

void TaskScheduler::AddTask(std::string id, std::shared_ptr<ITask> task)
{
  m_tasks.insert({id, task});
}

void TaskScheduler::RunTask(std::string id)
{
  if( m_tasks.find(id) != m_tasks.end() )
  {
    auto task = m_tasks.at(id);

    std::thread worker(&ITask::RunTask, task.get());
    worker.detach();
  }
}

void TaskScheduler::StopTask(std::string id)
{  
  if( m_tasks.find(id) != m_tasks.end() )
  {
    m_tasks.at(id)->StopTask();
  }
}

ITask::TaskStatus TaskScheduler::GetTaskStatus(std::string id)
{
  if( m_tasks.find(id) != m_tasks.end() )
    return m_tasks.at(id)->GetStatus();
  else
    return ITask::TaskStatus::NOT_RUN;
}