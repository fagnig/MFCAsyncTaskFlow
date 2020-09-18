#pragma once

#include "interface/IAsyncProgressUpdateable.h"

#include <unordered_map>

class UpdateableContainer
{
 public:
  using PrUpPtr = IAsyncProgressUpdateable *;

  struct StorageType
  {
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
  PrUpPtr GetProgressTarget(std::string id)                                       { try { return m_internalMap.at(id).ctrl; } catch(...) { return nullptr; } } 

  std::vector<PrUpPtr> GetProgressTargetByType(std::string typeToFind)
  {
    std::vector<PrUpPtr> ret;
    
    for( const auto [id, pack] : m_internalMap )
      if( pack.type == typeToFind )
        ret.push_back(pack.ctrl);

    return ret;
  }

 private:
  std::unordered_map<std::string, StorageType> m_internalMap;
};