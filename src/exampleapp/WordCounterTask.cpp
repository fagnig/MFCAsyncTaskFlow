
// MFCAsyncTaskFlowDlg.cpp : implementation file
//

#include "pch.h"

#include "exampleapp/WordCounterTask.h"

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

// Set to true to have the thread hang forever to test task cancellation
constexpr bool SHOULDTHREADHANG = false;

void WordCounterTask::RunTask()
{
  m_status = TaskStatus::RUNNING;

  int wordsfound = 0;
  int curline = 0;
  int linecount = 0;

  std::ifstream s;
  s.open(m_filepath, std::ifstream::in);

  if( s.is_open() )
  {
    std::string line;
    std::getline(s, line);
    const auto firstlinelen = line.length();
    s.seekg(0);

    const auto fsize = std::filesystem::file_size(m_filepath);
    const auto approxlines = fsize/firstlinelen;


    while(!s.eof()) {
      std::string dummy;
      std::getline(s, dummy);
      ++linecount;
      if( linecount % 5000 == 0 ){
        for( auto [id, pack] : m_updateables )
          if( id!="listlog" )
            pack.UpdateProgress((int)((linecount/(double)approxlines)*30));

        if( m_stoptoken )
        {
          s.close();
          StopAndCleanup();
          return;
        }
      }
    }

    std::string str = fmt::format("Found {} lines in '{}'.", linecount, m_filepath.string());
    std::wstring wstr = ATL::CA2W(str.c_str());
    m_updateables.GetProgressTarget("listlog").UpdateResult(wstr);

    if( m_stoptoken )
    {
      s.close();
      StopAndCleanup();
      return;
    }

    s.clear();
    s.seekg(0);

    while( std::getline(s, line) ){
      ++curline;
      size_t pos = 0;
      while( (pos = line.find(m_wordtofind, pos+1)) != std::string::npos )
        wordsfound++;

      if( curline % 200 == 0 ){
        if( m_stoptoken )
        {
          s.close();
          StopAndCleanup();
          return;
        }

        for( const auto [id, pack] : m_updateables ){
          if( id!="listlog" )
          {
            pack.ctrl->UpdateProgress(30 + (int)((curline/(double)linecount)*70));
          }
          else if( curline % 3000 == 0 )
          {
            std::string str = fmt::format("Searched {} out of {} lines in '{}'.", curline, linecount, m_filepath.string());
            std::wstring wstr = ATL::CA2W(str.c_str());
            m_updateables.GetProgressTarget("listlog").UpdateResult(wstr);
          }
        }
      }
    }
  }

  s.close();

  while( SHOULDTHREADHANG )
  {
    if( m_stoptoken )
    {
      StopAndCleanup();
      return;
    }
    std::this_thread::sleep_for(1s);
  }


  for( auto ctrl : m_updateables.GetProgressTargetByType("textual"))
  {
    std::string str = fmt::format("Found {} instances of the word '{}' in '{}', searched {} lines.", wordsfound, m_wordtofind, m_filepath.string(), linecount);
    std::wstring wstr = ATL::CA2W(str.c_str());
    ctrl.UpdateResult(wstr);
  }
  for( auto ctrl : m_updateables.GetProgressTargetByType("numeric"))
  {
    ctrl.UpdateResult(wordsfound);
  }

  m_status = TaskStatus::FINISHED;
}

void WordCounterTask::StopTask()
{
  m_stoptoken = true;
  m_status = TaskStatus::STOPPING;
  std::string str = "Stopping search...";
  std::wstring wstr = ATL::CA2W(str.c_str());
  m_updateables.GetProgressTarget("listlog").UpdateResult(wstr);
}

void WordCounterTask::HaltTask()
{
  //Log(LogType::Error, "WordCounterTask doesn't support halting");
}


void WordCounterTask::StopAndCleanup()
{
  m_status = TaskStatus::STOPPED;
  std::string str = fmt::format("Stopped searching for the word '{}'.", m_wordtofind);
  std::wstring wstr = ATL::CA2W(str.c_str());
  m_updateables.GetProgressTarget("listlog").UpdateResult(wstr);

  for( auto ctrl : m_updateables.GetProgressTargetByType("numeric")){
    ctrl.UpdateResult(0);
  }
  
  m_stoptoken = false;
}