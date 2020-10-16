#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <any>
#include <thread>
#include "utility/UpdateableContainer.h"

/// <summary>
/// Interface for implementing a task
/// The implementing class should make sure all data needed is kept alive through eg. std::shared_ptr's
/// 
/// </summary>
class ITask
{
 public:

  enum class TaskStatus   { NO_SUCH_TASK, NOT_RUN, RUNNING, HALTED, STOPPING, STOPPED, FINISHED };
  TaskStatus GetStatus()  { return m_status; };

  // Interface part starts here
  virtual void RunTask()  = 0; // Runs the task - This is the function that is run in the thread. 
                               // Once this function terminates, the task is finished, and will be freed from the internal map of the TaskScheduler,
                               // and the thread in the threadpool will be free to start another task.
  virtual void StopTask() = 0; // Stops the task - Sets the status to TaskStatus::STOPPING and sets m_stoptoken to true; it is up to the RunTask function to react to this status change.
  virtual void HaltTask() = 0; // Halts the task - Sets the status to TaskStatus::HALTED. it is up to the RunTask function to react to this status change.

 protected:
  UpdateableContainer m_updateables; // A collection of IAsyncProgressUpdateable pointers - used for updating user interface asynchronously 

  std::atomic<TaskStatus> m_status{TaskStatus::NOT_RUN};  // Status variable for keeping track of what is happening in the task.
  std::atomic<bool>       m_stoptoken{false};             // Token used for checking if the task should be stopping.

};