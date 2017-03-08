#pragma once
#ifndef BlockDinamicPart_h__
#define BlockDinamicPart_h__

#include <memory>
#include "Tesselator.h"
#include "agent/Agent.h"
#include <boost/container/flat_map.hpp>

class BlockDynamicPart
{
public:
  BlockDynamicPart();
  ~BlockDynamicPart();

  std::unique_ptr<BlockDynamicPart> Clone();

  std::unique_ptr<TesselatorData> mTesselatorData;

  boost::container::flat_map<AgentId, PAgent> mAgents;

public:
  void DrawGui(Magnum::Timeline dt) const;

public:
  template<class T>
  T *GetAgent()
  {
    AgentId name = T::TypeId();
    auto it = mAgents.find(name);
    if (it != mAgents.end())
    {
      return static_cast<T*>(it->second.get());
    }
    return nullptr;
  }
  template<class T>
  bool HasAgent()
  {
    AgentId & name = T::TypeId();
    auto it = mAgents.find(name);
    if (it != mAgents.end())
    {
      return true;
    }
    return false;
  }
};



#endif // BlockDinamicPart_h__
