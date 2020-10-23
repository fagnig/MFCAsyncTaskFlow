#pragma once

#include "interface/IAsyncProgressUpdateable.h"

#include <unordered_map>

class UpdateableContainer
{
 public:
  using PrUpPtr = IAsyncProgressUpdateable *;

  /// Internal type used for asynchronously updating
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

  /// @name Iterators for algorithms
  ///@{ 
  auto begin()              { return m_internalMap.begin(); }
  auto end()                { return m_internalMap.end();   }
  const auto begin() const  { return m_internalMap.begin(); }
  const auto end() const    { return m_internalMap.end();   }
  ///@}

  /// @brief Add a control to the container.
  /// @param id Specific identifier for getting a specific control.
  /// @param type Type identifier, for grouping the same kind of control together, eg. all "textual" or all "numeric" controls.
  /// @param hwnd Window handle (HWND) for the control. The underlying control should handle ASYNCUPDATEPROGRESS_MESSAGE and ASYNCUPDATERESULT_MESSAGE messages.
  void AddProgressUpdateTarget(std::string id, std::string type, HWND hwnd)     { m_internalMap.insert({id, {type, hwnd}}); }
  
  /// @brief Erase a progress target from the container with given string identifier.
  /// @param id Target identifier to remove.
  /// @return true if a target was removed, false if not.
  bool RemoveProgressUpdateTarget(std::string id)
  { 
    return m_internalMap.erase(id) > 0; 
  }

  /// @brief Get a specific progress target.
  /// @param id Target identifier to get.
  /// @return A storage struct containing the type and the HWND of a control.
  [[ nodiscard ]]
  StorageType GetProgressTarget(std::string id) 
  { 
    if(m_internalMap.find(id) != m_internalMap.end()){
      return m_internalMap.at(id); 
    }

    return {"nulltype", NULL};
  } 

  /// @brief Get a collection of progress targets by their type.
  /// @param typeToFind Type identifier to get.
  /// @return A vector of storage structs containing the type string identifier and the HWND of a control.
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