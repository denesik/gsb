#pragma once
#ifndef BlockDinamicPart_h__
#define BlockDinamicPart_h__

#include "TemplateFactory.h"
#include <memory>
#include "Tesselator.h"
#include "accessors/Accessor.h"
#include <boost/container/flat_map.hpp>
#include "IGui.h"
#include "StaticBlock.h"
#include <boost/optional/optional.hpp>
#include "tools/CoordSystem.h"
#include <array>
#include <vector>
#include <rapidjson/rapidjson.h>

// TODO: clone std::unique_ptr --> move semantic


class DataBase;
class Sector;

// Интерфейс блока. Нельзя создать.
class Block : public IGui
{
public:
  using factory = TemplateFactory<std::string, Block, void(const DataBase &, const rapidjson::Value &)>;

  // TODO: Скорей всего надо передавать сектор вместо бд.
  Block();
  Block(const DataBase & db, const rapidjson::Value &json);
  Block(const Block &other);
  ~Block();

  virtual std::unique_ptr<Block> Clone();

  virtual void Update(const Magnum::Timeline &dt) {};

  void DrawGui(const Magnum::Timeline &dt) override;

  // TODO: Сделать JsonLoad и убрать туда это.
  bool AddAgent(std::unique_ptr<Accessor> accessor);

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
  //boost::container::flat_multimap<AgentId, std::unique_ptr<Agent>> mAgents;
  std::vector<std::unique_ptr<Accessor>> mAgents;
};

#endif // BlockDinamicPart_h__
