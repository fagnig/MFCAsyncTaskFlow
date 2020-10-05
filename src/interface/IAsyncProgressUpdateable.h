#pragma once

#include <any>

/// <summary>
/// Interface for allowing a MFC control to be updated asynchronously.
/// UpdateProgress can called as many times as one would like. 
/// UpdateResult should be called once, ideally at the end of the task.
/// See CProgressBarUpdateAble for an example of an implementation of this interface.
/// 
/// </summary>

constexpr int ASYNCUPDATEPROGRESS_MESSAGE = WM_USER + 500;
constexpr int ASYNCUPDATERESULT_MESSAGE = WM_USER + 501;

class IAsyncProgressUpdateable
{
public:
  virtual void UpdateProgress(std::any in) = 0;

  virtual void UpdateResult(std::any in) = 0;

};


// Below are macros defined for implementation of the message passing style of asynchronous updating

#define MAKE_ASYNCUPDATEABLE(t, base_t)                                         \
  BEGIN_MESSAGE_MAP( t, base_t )                                                \
    ON_MESSAGE( ASYNCUPDATEPROGRESS_MESSAGE, &t::UpdateProgressMessage )        \
    ON_MESSAGE( ASYNCUPDATERESULT_MESSAGE, &t::UpdateResultMessage )            \
  END_MESSAGE_MAP()                                                             \
                                                                                \
  LRESULT t::UpdateProgressMessage(WPARAM w, LPARAM l)                          \
  {                                                                             \
    std::any * in = (std::any *)(l);                                            \
                                                                                \
    UpdateProgress(*in);                                                        \
    delete in;                                                                  \
    return 0;                                                                   \
  }                                                                             \
                                                                                \
  LRESULT t::UpdateResultMessage(WPARAM w, LPARAM l)                            \
  {                                                                             \
    std::any * in = (std::any *)(l);                                            \
                                                                                \
    UpdateResult(*in);                                                          \
    delete in;                                                                  \
    return 0;                                                                   \
  }                                                                             \

#define MAKE_ASYNCUPDATEABLE_HEADER()                                           \
  DECLARE_MESSAGE_MAP();                                                        \
  afx_msg LRESULT UpdateProgressMessage(WPARAM, LPARAM);                        \
  afx_msg LRESULT UpdateResultMessage(WPARAM, LPARAM);                          \
