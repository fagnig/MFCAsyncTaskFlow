#pragma once

#include "interface/IAsyncProgressUpdateable.h"

#include <unordered_map>

class UpdateableContainer
{
 public:
  using PrUpPtr = IAsyncProgressUpdateable *;

  // Internal 
  struct StorageType
  {
    void UpdateProgress(std::any in)
    {
      auto tmp = new std::any;
      *tmp = in;

      PostMessage(ctrlhwnd, ASYNCUPDATEPROGRESS_MESSAGE, 0, (LPARAM)tmp);
    }
    void UpdateResult(std::any in)
    {
      auto tmp = new std::any;
      *tmp = in;

      PostMessage(ctrlhwnd, ASYNCUPDATERESULT_MESSAGE, 0, (LPARAM)tmp);
    }
    std::string type;
    HWND        ctrlhwnd;
  };

  // Iterators for algorithms
  auto begin()              { return m_internalMap.begin(); }
  auto end()                { return m_internalMap.end();   }
  const auto begin() const  { return m_internalMap.begin(); }
  const auto end() const    { return m_internalMap.end();   }

  // Functions for interacting with the contents of the container
  void    AddProgressUpdateTarget(std::string id, std::string type, HWND hwnd)     { m_internalMap.insert({id,{type ,hwnd}}); }
  void    RemoveProgressUpdateTarget(std::string id)                              { m_internalMap.erase(id); }

  [[ nodiscard ]] 
  StorageType GetProgressTarget(std::string id) 
  { 
    if(m_internalMap.find(id) != m_internalMap.end()){
      return m_internalMap.at(id); 
    }

    return {"nulltype", nullptr};
  } 

  [[ nodiscard ]] 
  std::vector<StorageType> GetProgressTargetByType(std::string typeToFind)
  {
    std::vector<StorageType> ret;
    
    for( const auto [id, pack] : m_internalMap )
      if( pack.type == typeToFind )
        ret.push_back(pack);

    return ret;
  }

 private:
  std::unordered_map<std::string, StorageType> m_internalMap;
};