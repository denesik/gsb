#pragma once
#ifndef BlockDinamicPart_h__
#define BlockDinamicPart_h__

#include <memory>
#include "Tesselator.h"
#include "agent/Agent.h"
#include <boost/container/flat_map.hpp>
#include "IGui.h"
#include "BlockStaticPart.h"

class BlocksDataBase;

class BlockDynamicPart : public IGui
{
public:
  // TODO: Скорей всего надо передавать сектор вместо бд.
  BlockDynamicPart(BlockId id, const BlocksDataBase &db);
  BlockDynamicPart(const BlockDynamicPart &other);
  ~BlockDynamicPart();

  std::unique_ptr<BlockDynamicPart> Clone();

  void DrawGui(const Magnum::Timeline &dt) override;

  std::unique_ptr<TesselatorData> &GetTesselatorData();
  const std::unique_ptr<TesselatorData> &GetTesselatorData() const;

  bool AddAgent(std::unique_ptr<Agent> agent);

  const std::unique_ptr<BlockStaticPart> &GetStaticPart() const;

  const BlocksDataBase &GetDataBase() const;

private:
  const BlocksDataBase &mDb;
  const BlockId mBlockId;
  std::unique_ptr<TesselatorData> mTesselatorData;
  boost::container::flat_map<AgentId, std::unique_ptr<Agent>> mAgents;

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
