#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <any>
#include <thread>
#include <mutex>
#include "utility/UpdateableContainer.h"


/// \mainpage Index
/// 
/// \section intro_sec Introduction
/// 
/// This is the documentation for the MFC Async Taskflow project. \n
/// See *LINK TO REPORT HERE* for a more detailed report of the project. \n
/// 
/// \section class_sec Classes of interest
///   Below is a list of classes that constitute the main part of the project, whereas the rest are mostly prototype and implementation.
/// 
///   - ITask
///   - IAsyncProgressUpdateable.h
///   - TaskScheduler
///   - UpdateableContainer
/// 



/// Interface for implementing a task \n
/// The implementing class should make sure all data needed is kept alive through eg. std::shared_ptr's.

class ITask
{
 public:

  /// @brief Status enumeration for keeping track of 
  enum class TaskStatus   { 
                            NO_SUCH_TASK, ///< Used to denote an invalid state - actual tasks should never have this as their status
                            NOT_RUN,      ///< Task has not yet been run
                            RUNNING,      ///< Task is currently running
                            SUSPENDED,    ///< Task was running, but has been suspended
                            STOPPING,     ///< Task is trying to stop
                            STOPPED,      ///< Task has stopped
                            FINISHED      ///< Task finished normally
                          };

  /// @brief Gets the status of the task
  /// @return The status of the task
  TaskStatus GetStatus()  { return m_status; };

  /// @name Interface - implement these in tasks
  ///@{ 

  /// @brief Runs the task - This is the function that is run in the thread. 
  /// Once this function terminates, the task is finished, and will be freed from the internal map of the TaskScheduler,
  /// and the thread in the threadpool will be free to start another task.
  virtual void RunTask()      = 0; 
  virtual void OnSuspend()    = 0; ///< @brief Function to run when suspending ; can be used for updating ui to notify user of a resumed task
  virtual void OnResume()     = 0; ///< @brief Function to run when resuming ; can be used for updating ui to notify user of a resumed task
  virtual void OnStopping()   = 0; ///< @brief Function to run when stopping ; can be used for reporting stopping if cleanup could take a while
  virtual void OnStopped()    = 0; ///< @brief Function to run when task is completely stopped ; can be used for updating/resetting ui
  ///@}


  /// @name Execution Control
  ///@{ 

  /// @brief Stops the task - Sets the status to TaskStatus::STOPPING and m_stoptoken to true. \n
  ///                         OnStopped() will be called when the next StopPoint() is reached in RunTask(). \n
  ///                         Can be called directly or through the TaskScheduler::SuspendTask().
  void StopTask()
  {
    OnStopping();
    m_stoptoken = true;
    m_status = TaskStatus::STOPPING;
  }
  
  /// @brief Suspends the task - Will set the status to TaskStatus::SUSPENDED. \n
  ///                            OnSuspend() will be called when the next SuspendPoint() is reached in RunTask(). \n
  ///                            Can be called directly or through the TaskScheduler::SuspendTask().
  void SuspendTask() {
    m_suspendtoken = true;
  }

  /// @brief Resumes a suspended task - Will let RunTask() resume running from the last encountered SuspendPoint(). \n
  ///                                   OnResume() will be called if the task was suspended. \n
  ///                                   Can be called directly or through the TaskScheduler::SuspendTask().
  void ResumeTask()
  {
    m_suspendtoken = false;
    m_suspendcondition.notify_all();
  }
  ///@}

  /// @name Flow Control
  /// @{ 
  
  /// @brief To be called by the RunTask() implementation to denote a point where stopping is possible.
  /// @return True if the function should stop running, false if not. 
  bool StopPoint() {
    if( m_stoptoken ) {
      OnStopped();
      m_status = TaskStatus::STOPPED;
      return true;
    }
    return false;
  }

  /// @brief To be called by the RunTask() implementation to denote a point where suspending is possible.
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
  ///@}
  
 protected:
  /// @brief A collection of updateable controls - used for updating user interface asynchronously 
  UpdateableContainer     m_updateables; 
  
  std::atomic<TaskStatus> m_status{TaskStatus::NOT_RUN};  ///< @brief Status variable for keeping track of what is happening in the task.
  std::atomic<bool>       m_stoptoken{false};             ///< @brief Token used for checking if the task should be stopping.
  std::atomic<bool>       m_suspendtoken{false};          ///< @brief Token used for checking if the task should be suspended.

  ///@{ 
  /// @name Internal variables used for suspending task efficiently
  std::mutex              m_suspendmutex;
  std::condition_variable m_suspendcondition;
  ///@}
};