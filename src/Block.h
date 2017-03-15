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
  using factory = TemplateFactory<std::string, Block, void(const DataBase &, const rapidjson::Value &, Sector &, BlockId)>;

  Block() = delete;
  virtual ~Block() = default;

  Block(const Block &other);
  Block(Block &&other);
  /// Не используем операторы копирования и перемещения.
  const Block &operator=(const Block &other) = delete;
  Block &operator=(Block &&other) = delete;

  /// Конструкторы для клонирования.
  Block(const Block &other, Sector &parent);
  Block(Block &&other, Sector &parent);

  /// Создаем элемент через фабрику.
  Block(const DataBase &db, const rapidjson::Value &val, Sector &parent, BlockId id);

  virtual std::unique_ptr<Block> Clone(Sector &parent);

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

  void SetPos(IndexType pos);

public:
  Sector &m_sector;
  const DataBase &mDb;
  BlockId mBlockId;

protected:
  IndexType mPos;

protected:
  //boost::container::flat_multimap<AgentId, std::unique_ptr<Agent>> mAgents;
  std::vector<std::unique_ptr<Accessor>> mAgents;
};

#endif // BlockDinamicPart_h__
