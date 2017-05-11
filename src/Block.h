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
#include <Magnum/Magnum.h>
#include <Magnum/AbstractShaderProgram.h>

// TODO: clone std::unique_ptr --> move semantic


class DataBase;
class Sector;

// Интерфейс блока. Нельзя создать.
class Block : public NoContextGui
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

  virtual std::unique_ptr<Block> Clone(Sector &parent) = 0;

  virtual void Update(const Magnum::Timeline &dt) = 0;

  virtual void Draw(const Magnum::Matrix4 &vp, Magnum::AbstractShaderProgram& shader) = 0;

  // Вероятно этот метод protected.
  const std::unique_ptr<StaticBlock> &GetStaticPart() const;

  const DataBase &GetDataBase() const;

  /// Получить интерфейс по заданному типу интерфейса, стороне и направления.
  boost::optional<Accessor &> GetAgent(AccessorId type, SideIndex side, AccessorDirection dir);

  Block *GetNeighbour(SideIndex side);

  void SetPos(IndexType pos);

  /// Получить ссылку на акцессор по его имени.
  boost::optional<Accessor &> GetAccessorByName(AccessorId name) const;
  boost::optional<Accessor &> GetAccessorByName(const std::string &name) const;

protected:
  Sector &m_sector;
  const DataBase &mDb;
  BlockId mBlockId;
  IndexType mPos;
  std::vector<std::unique_ptr<Accessor>> mAgents;
};

#endif // BlockDinamicPart_h__
