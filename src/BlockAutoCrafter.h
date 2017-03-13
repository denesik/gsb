#pragma once
#ifndef BlockAutoCrafter_h__
#define BlockAutoCrafter_h__


#include "Block.h"
#include "Timer.h"
#include "IRecipe.h"
#include "Crafter.h"

// ��������� � �� � ������� �������.
// ��������� ����� �������� ����� json ��� ��������.
// ��� �������� � ���� �����������.

class BlockAutoCrafter;
namespace
{
  static auto item_loaded = Block::factory::Register<BlockAutoCrafter>::add("BlockAutoCrafter");
}

// ���� ����������.
// ���������� 2 ��������.
class BlockAutoCrafter : public Block
{
public:
  BlockAutoCrafter() = delete;
  ~BlockAutoCrafter();
  BlockAutoCrafter(const DataBase & db, const rapidjson::Value &json);
  BlockAutoCrafter(const BlockAutoCrafter &other);

  std::unique_ptr<Block> Clone() override;

  void DrawGui(const Magnum::Timeline &dt) override;

  void Update(const Magnum::Timeline &dt) override;

private:
  Crafter mCrafter;
  Crafter mGenerator;

private:
  std::unique_ptr<IRecipe> CrafterType(const char *type, const DataBase & db, const rapidjson::Value &json) const;
  bool CrafterFast(const char *type, const DataBase & db, const rapidjson::Value &json) const;
};

#endif // BlockAutoCrafter_h__
