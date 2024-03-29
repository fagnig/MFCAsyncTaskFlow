
// WordCounterTask.h : header file
//

#pragma once

#include "interface/ITask.h"
#include "utility/UpdateableContainer.h"

#include <filesystem>
#include <fstream>

class WordCounterTask : public ITask
{
public:
  WordCounterTask(std::string wordtofind, std::filesystem::path filepath ,UpdateableContainer updateables):
    m_wordtofind(wordtofind), m_filepath(filepath), m_updateables(updateables) {};

  void RunTask()    override;
  void OnSuspend()  override;
  void OnResume()   override;
  void OnStopping() override;
  void OnStopped()  override;

  std::filesystem::path m_filepath;
  std::string           m_wordtofind;
  UpdateableContainer   m_updateables;

private:
  std::ifstream         m_ifstream;
};
