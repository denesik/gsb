#pragma once
#ifndef BlockDinamicPart_h__
#define BlockDinamicPart_h__

#include "TemplateFactory.h"
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

// 
class BlockDynamicPart : public IGui
{
public:
  // TODO: Скорей всего надо передавать сектор вместо бд.
  BlockDynamicPart();
  BlockDynamicPart(const BlockDynamicPart &other);
  ~BlockDynamicPart();

  virtual std::unique_ptr<BlockDynamicPart> Clone();

  virtual void Update() {};

  void DrawGui(const Magnum::Timeline &dt) override;

  std::unique_ptr<TesselatorData> &GetTesselatorData();
  const std::unique_ptr<TesselatorData> &GetTesselatorData() const;

  // TODO: Сделать JsonLoad и убрать туда это.
  bool AddAgent(std::unique_ptr<Agent> agent); 

  // Вероятно этот метод protected.
  const std::unique_ptr<BlockStaticPart> &GetStaticPart() const;

  const BlocksDataBase &GetDataBase() const;

  /// Получить интерфейс по заданному типу интерфейса, стороне и направления.
  boost::optional<Agent &> GetAgent(AgentId type, SideIndex side, AgentDirection dir);

  BlockDynamicPart *GetNeighbour(SideIndex side);

public:
  IndexType pos;
  Sector *m_sector;

public:
  BlocksDataBase *mDb;
  BlockId mBlockId;
protected:
  std::unique_ptr<TesselatorData> mTesselatorData;
  //boost::container::flat_multimap<AgentId, std::unique_ptr<Agent>> mAgents;
  std::vector<std::unique_ptr<Agent>> mAgents;
};




#define REGISTER_BLOCK_CLASS(type) REGISTER_ELEMENT(type, BlockFactory::Get(), #type)

struct BlockFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory<std::string, BlockDynamicPart>;
  static FactoryType &Get();
};

#endif // BlockDinamicPart_h__
