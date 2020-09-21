#pragma once

#include "interface/IAsyncProgressUpdateable.h"

#include <unordered_map>

class UpdateableContainer
{
 public:
  using PrUpPtr = IAsyncProgressUpdateable *;

  struct StorageType
  {
    void UpdateProgress(std::any in)
    {
      if( ctrl )
        ctrl->UpdateProgress(in);
    }
    void UpdateResult(std::any in)
    {
      if( ctrl )
        ctrl->UpdateResult(in);
    }
    std::string type;
    PrUpPtr     ctrl;
  };

  // Iterators for algorithms
  auto begin()              { return m_internalMap.begin(); }
  auto end()                { return m_internalMap.end();   }
  const auto begin() const  { return m_internalMap.begin(); }
  const auto end() const    { return m_internalMap.end();   }

  // Functions for interacting with the contents of the container
  void    AddProgressUpdateTarget(std::string id, std::string type, PrUpPtr ptr)  { m_internalMap.insert({id,{type, ptr}}); }
  void    RemoveProgressUpdateTarget(std::string id)                              { m_internalMap.erase(id); }
  StorageType GetProgressTarget(std::string id) 
  { 
    if(m_internalMap.find(id) != m_internalMap.end()){
      return m_internalMap.at(id); 
    }

    return {"nulltype", nullptr};
  } 

  std::vector<StorageType> GetProgressTargetByType(std::string typeToFind)
  {
    std::vector<StorageType> ret;
    
    for( const auto [id, pack] : m_internalMap )
      if( pack.type == typeToFind && pack.ctrl != nullptr )
        ret.push_back(pack);

    return ret;
  }

 private:
  std::unordered_map<std::string, StorageType> m_internalMap;
};