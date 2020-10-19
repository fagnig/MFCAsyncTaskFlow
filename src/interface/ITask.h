#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <any>
#include <thread>
#include <mutex>
#include "utility/UpdateableContainer.h"

/// <summary>
/// Interface for implementing a task
/// The implementing class should make sure all data needed is kept alive through eg. std::shared_ptr's
/// 
/// </summary>
class ITask
{
 public:

  enum class TaskStatus   { NO_SUCH_TASK, NOT_RUN, RUNNING, SUSPENDED, STOPPING, STOPPED, FINISHED };
  TaskStatus GetStatus()  { return m_status; };

  // Interface part starts here
  virtual void RunTask()      = 0;  // Runs the task - This is the function that is run in the thread. 
                                    // Once this function terminates, the task is finished, and will be freed from the internal map of the TaskScheduler,
                                    // and the thread in the threadpool will be free to start another task.
  
  // Stops the task - Sets the status to TaskStatus::STOPPING and m_stoptoken to true;
  //                  OnStop will be called when the next StopPoint() is reached in RunTask()
  void StopTask()
  {
    OnStopping();
    m_stoptoken = true;
    m_status = TaskStatus::STOPPING;
  }
  
  // Suspends the task - Will set the status to TaskStatus::SUSPENDED when the next SuspendPoint() is reached in RunTask()
  void SuspendTask() {
    m_suspendtoken = true;
  }

  // Resumes a suspended task
  virtual void ResumeTask()
  {
    m_suspendtoken = false;
    m_suspendcondition.notify_all();
  }

  bool StopPoint() {
    if( m_stoptoken ) {
      OnStopped();
      m_status = TaskStatus::STOPPED;
      return true;
    }
    return false;
  }

  void SuspendPoint() {
    if( m_suspendtoken )
    {
      std::unique_lock<std::mutex> lk(m_suspendmutex);

      OnSuspend();
      m_status = TaskStatus::SUSPENDED;

      m_suspendcondition.wait(lk, [this](){return !m_suspendtoken.load();});

      OnResume();
      m_status = TaskStatus::RUNNING;
    }
  }

  virtual void OnSuspend()    = 0; // Function to run when suspending
  virtual void OnResume()     = 0; // Function to run when resuming
  virtual void OnStopping()   = 0; // Function to run when stopping
  virtual void OnStopped()    = 0; 

 protected:
  UpdateableContainer     m_updateables; // A collection of updateable controls - used for updating user interface asynchronously 

  std::atomic<TaskStatus> m_status{TaskStatus::NOT_RUN};  // Status variable for keeping track of what is happening in the task.
  std::atomic<bool>       m_stoptoken{false};             // Token used for checking if the task should be stopping.
  std::atomic<bool>       m_suspendtoken{false};          // Token used for suspending a task

  std::mutex              m_suspendmutex;
  std::condition_variable m_suspendcondition;
};