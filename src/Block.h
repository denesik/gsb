#pragma once
#ifndef BlockDinamicPart_h__
#define BlockDinamicPart_h__

#include "TemplateFactory.h"
#include <memory>
#include "Tesselator.h"
#include "agent/Accessor.h"
#include <boost/container/flat_map.hpp>
#include "IGui.h"
#include "StaticBlock.h"
#include <boost/optional/optional.hpp>
#include "tools/CoordSystem.h"
#include <array>
#include <vector>

// TODO: clone std::unique_ptr --> move semantic


class DataBase;
class Sector;

// 
class Block : public IGui
{
public:
  // TODO: Скорей всего надо передавать сектор вместо бд.
  Block();
  Block(const Block &other);
  ~Block();

  virtual std::unique_ptr<Block> Clone();

  virtual void Update(const Magnum::Timeline &dt) {};

  void DrawGui(const Magnum::Timeline &dt) override;

  std::unique_ptr<TesselatorData> &GetTesselatorData();
  const std::unique_ptr<TesselatorData> &GetTesselatorData() const;

  // TODO: Сделать JsonLoad и убрать туда это.
  bool AddAgent(std::unique_ptr<Accessor> agent); 

  // Вероятно этот метод protected.
  const std::unique_ptr<StaticBlock> &GetStaticPart() const;

  const DataBase &GetDataBase() const;

  /// Получить интерфейс по заданному типу интерфейса, стороне и направления.
  boost::optional<Accessor &> GetAgent(AccessorId type, SideIndex side, AccessorDirection dir);

  Block *GetNeighbour(SideIndex side);

public:
  IndexType pos;
  Sector *m_sector;

public:
  DataBase *mDb;
  BlockId mBlockId;
protected:
  std::unique_ptr<TesselatorData> mTesselatorData;
  //boost::container::flat_multimap<AgentId, std::unique_ptr<Agent>> mAgents;
  std::vector<std::unique_ptr<Accessor>> mAgents;
};




#define REGISTER_BLOCK_CLASS(type) REGISTER_ELEMENT(type, BlockFactory::Get(), #type)

struct BlockFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory<std::string, Block>;
  static FactoryType &Get();
};

#endif // BlockDinamicPart_h__
