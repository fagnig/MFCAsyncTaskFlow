#pragma once

#include "interface/XIAsyncProgressUpdateable.h"

#include <unordered_map>

class UpdateableContainer
{
 public:
  using PrUpPtr = XIAsyncProgressUpdateable *;

  // Iterators for algorithms
  auto begin()              { return m_internalMap.begin(); }
  auto end()                { return m_internalMap.end();   }
  const auto begin() const  { return m_internalMap.begin(); }
  const auto end() const    { return m_internalMap.end();   }

  // Functions for interacting with the contents of the container
  void    AddProgressUpdateTarget(std::string id, PrUpPtr ptr)    { m_internalMap.insert({id, ptr}); }
  void    RemoveProgressUpdateTarget(std::string id)              { m_internalMap.erase(id); }
  PrUpPtr GetProgressTarget(std::string id)                       { try { return m_internalMap.at(id); } catch(...) { return nullptr; } } 

 private:
  std::unordered_map<std::string, PrUpPtr> m_internalMap;
};

