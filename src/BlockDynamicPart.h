#pragma once
#ifndef BlockDinamicPart_h__
#define BlockDinamicPart_h__

#include <memory>
#include "Tesselator.h"
#include "agent/Agent.h"
#include <boost/container/flat_map.hpp>
#include "IGui.h"
#include "BlockStaticPart.h"
#include <boost/optional/optional.hpp>
#include "tools/CoordSystem.h"
#include <array>
#include <vector>

// TODO: clone std::unique_ptr --> move semantic


class BlocksDataBase;
class Sector;

// К каждой стороне блока можно прибиндить несколько интерфейсов разного типа и указать направление.
// 
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

  /// Получить интерфейс по заданному типу интерфейса, стороне и направления.
  boost::optional<Agent &> GetAgent(AgentId type, SideIndex side, AgentDirection dir);

  BlockDynamicPart *GetNeighbour(SideIndex side);

public:
  IndexType pos;
  Sector *m_sector;

private:
  const BlocksDataBase &mDb;
  const BlockId mBlockId;
  std::unique_ptr<TesselatorData> mTesselatorData;
  //boost::container::flat_multimap<AgentId, std::unique_ptr<Agent>> mAgents;
  std::vector<std::unique_ptr<Agent>> mAgents;
};



#endif // BlockDinamicPart_h__
