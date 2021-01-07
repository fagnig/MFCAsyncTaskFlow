
// MFCAsyncTaskFlowDlg.cpp : implementation file
//

#include "pch.h"

#include "exampleapp/WaiterTask.h"

#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>
#include <array>
#include <fstream>
#include <sstream>
#include <iostream>

#include "fmt/core.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std::chrono_literals;

#pragma optimize( "", off )
void WaiterTask::RunTask()
{
  m_status = TaskStatus::RUNNING;

  m_updateables.GetProgressTarget("throbberbutton").UpdateProgress(1);

  auto donetimepoint = std::chrono::high_resolution_clock::now() + std::chrono::seconds(m_timetowait);


  while( !StopPoint() )
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if(std::chrono::high_resolution_clock::now() > donetimepoint) break;
  }


  m_updateables.GetProgressTarget("throbberbutton").UpdateResult(1);

  std::wstring wstr = L"Finished asynchronously waiting.";
  m_updateables.GetProgressTarget("listlog").UpdateResult(wstr);

  m_status = TaskStatus::FINISHED;
}
#pragma optimize( "", on )

void WaiterTask::OnStopping()
{
  //Log(LogType::Error, "WaiterTask doesn't support stopping");
}

void WaiterTask::OnStopped()
{
  //Log(LogType::Error, "WaiterTask doesn't support stopping");
}

void WaiterTask::OnSuspend()
{
  //Log(LogType::Error, "WaiterTask doesn't support suspending");
}

void WaiterTask::OnResume()
{
  //Log(LogType::Error, "WaiterTask doesn't support resuming");
}