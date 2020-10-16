#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <thread>
#include <memory>
#include <queue>
#include <mutex>

#include "utility/UpdateableContainer.h"
#include "interface/ITask.h"


/// <summary>
/// Global task scheduler
/// Operates on a thread-pool basis, with a number of threads contained in a pool (m_threadPool) 
/// running a spinner-type function until a task is pushed into the queue (m_taskQueue),
/// whereafter the thread will add the task to the map of running tasks (m_runningTasks)
/// 
/// </summary>
class TaskScheduler
{
 public:
  using QueueTuple = std::tuple<std::string, std::shared_ptr<ITask>>;

  TaskScheduler();
  ~TaskScheduler();

  void AddTask(std::string id, std::shared_ptr<ITask> task);
  void StopTask(std::string id);
  ITask::TaskStatus GetTaskStatus(std::string id);

  // Should be called at the end of the program; will stop all tasks and attempt to join them
  void ShutdownScheduler();

 private:
  void TaskSpinner();
  bool m_stopped{false};

  std::unordered_map<std::string, std::shared_ptr<ITask>> m_runningTasks;

  std::vector<std::thread>            m_threadPool;

  // Everything to do with the queue below here:
  std::queue<QueueTuple>              m_taskQueue;
  std::mutex                          m_queuelock;
  std::condition_variable             m_condition;
  std::atomic<bool>                   m_killworkers{false};

};

static TaskScheduler g_globalTaskManager;