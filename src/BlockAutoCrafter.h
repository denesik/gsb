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

// Содержит 2 крафтера.
class BlockAutoCrafter : public Block
{
public:
  BlockAutoCrafter();
  ~BlockAutoCrafter();
  BlockAutoCrafter(const DataBase & db, const rapidjson::Value &json);
  BlockAutoCrafter(const BlockAutoCrafter &other);

  std::unique_ptr<Block> Clone() override;

  void DrawGui(const Magnum::Timeline &dt) override;

  void Update(const Magnum::Timeline &dt) override;

private:
  Crafter mCrafter;
  Crafter mGenerator;
};

REGISTER_BLOCK_CLASS(BlockAutoCrafter);

#endif // BlockAutoCrafter_h__
