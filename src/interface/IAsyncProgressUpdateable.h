#pragma once

#include <any>

/// @file IAsyncProgressUpdateable.h
/// Interface for allowing a MFC control to be updated asynchronously. \n
/// UpdateProgress can called as many times as one would like. \n
/// UpdateResult should be called once, ideally at the end of the task. \n
/// See CProgressBarUpdateAble for an example of an implementation of this interface. \n
/// Below are macros defined for implementation of the message passing style of asynchronous updating for MFC controls. \n
/// MAKE_ASYNCUPDATEABLE(t, base_t) is the implementation macro - With more complex classes (that already have a message map), \n
/// these should be implemented using MAKE_ASYNCUPDATEABLE_MSGMAP and MAKE_ASYNCUPDATEABLE_FUNCS macros like so: \n
/// @code{.cpp}
///                           BEGIN_MESSAGE_MAP(TestClass, BaseClass)
///                             -- Other messagemap functions --
///                             MAKE_ASYNCUPDATEABLE_MSGMAP(TestClass)
///                           END_MESSAGE_MAP()
/// 
///                           MAKE_ASYNCUPDATEABLE_FUNCS(TestClass)
/// @endcode
/// Same idea with the header definitions, MAKE_ASYNCUPDATEABLE_HEADER() if no messagemap, \n
/// MAKE_ASYNCUPDATEABLE_HEADER_NOMSGMAP() if messagemap is already present. \n


constexpr int ASYNCUPDATEPROGRESS_MESSAGE = WM_USER + 500;
constexpr int ASYNCUPDATERESULT_MESSAGE   = WM_USER + 501;

/// @brief See IAsyncProgressUpdateable.h
class IAsyncProgressUpdateable
{
public:
  virtual void UpdateProgress(std::any in) = 0;

  virtual void UpdateResult  (std::any in) = 0;

};

#define MAKE_ASYNCUPDATEABLE_MSGMAP(t)                                          \
    ON_MESSAGE( ASYNCUPDATEPROGRESS_MESSAGE, &t::UpdateProgressMessage )        \
    ON_MESSAGE( ASYNCUPDATERESULT_MESSAGE, &t::UpdateResultMessage )            \

#define MAKE_ASYNCUPDATEABLE_FUNCS(t)                                           \
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

#define MAKE_ASYNCUPDATEABLE(t, base_t)                                         \
  BEGIN_MESSAGE_MAP( t, base_t )                                                \
    MAKE_ASYNCUPDATEABLE_MSGMAP(t)                                              \
  END_MESSAGE_MAP()                                                             \
                                                                                \
  MAKE_ASYNCUPDATEABLE_FUNCS(t)                                                 \

#define MAKE_ASYNCUPDATEABLE_HEADER_NOMSGMAP()                                  \
  afx_msg LRESULT UpdateProgressMessage(WPARAM, LPARAM);                        \
  afx_msg LRESULT UpdateResultMessage(WPARAM, LPARAM);                          \

#define MAKE_ASYNCUPDATEABLE_HEADER()                                           \
  DECLARE_MESSAGE_MAP()                                                         \
  MAKE_ASYNCUPDATEABLE_HEADER_NOMSGMAP()                                        \
