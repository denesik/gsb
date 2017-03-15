#pragma once
#ifndef BlockAutoCrafter_h__
#define BlockAutoCrafter_h__


#include "Block.h"
#include "Timer.h"
#include "IRecipe.h"
#include "Crafter.h"

// Создается в БД с помощью фабрики.
// Настройки блока задаются через json при создании.
// Для создания в мире клонируется.

class BlockAutoCrafter;
namespace
{
  static auto item_loaded = Block::factory::Register<BlockAutoCrafter>::add("BlockAutoCrafter");
}

// Блок автокрафта.
// Использует 2 крафтера.
class BlockAutoCrafter : public Block
{
public:
  BlockAutoCrafter() = delete;
  virtual ~BlockAutoCrafter() = default;

  BlockAutoCrafter(const BlockAutoCrafter &other);
  BlockAutoCrafter(BlockAutoCrafter &&other);
  /// Не используем операторы копирования и перемещения.
  const BlockAutoCrafter &operator=(const BlockAutoCrafter &other) = delete;
  BlockAutoCrafter &operator=(BlockAutoCrafter &&other) = delete;

  /// Конструкторы для клонирования.
  BlockAutoCrafter(const BlockAutoCrafter &other, Sector &parent);
  BlockAutoCrafter(BlockAutoCrafter &&other, Sector &parent);

  /// Создаем элемент через фабрику.
  BlockAutoCrafter(const DataBase &db, const rapidjson::Value &val, Sector &parent, BlockId id);

  std::unique_ptr<Block> Clone(Sector &parent) override;

  void DrawGui(const Magnum::Timeline &dt) override;

  void Update(const Magnum::Timeline &dt) override;

private:
  Crafter mCrafter;
  Crafter mGenerator;

private:
  IRecipe::Tag CrafterType(const char *type, const DataBase & db, const rapidjson::Value &json) const;
  bool CrafterFast(const char *type, const DataBase & db, const rapidjson::Value &json) const;
};

#endif // BlockAutoCrafter_h__
