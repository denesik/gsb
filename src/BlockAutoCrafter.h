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
  static auto BlockAutoCrafterLoaded = Block::factory::Register<BlockAutoCrafter>::add("BlockAutoCrafter");
}

// ���� ����������.
// ���������� 2 ��������.
class BlockAutoCrafter : public Block
{
public:
  BlockAutoCrafter() = delete;
  virtual ~BlockAutoCrafter() = default;

  BlockAutoCrafter(const BlockAutoCrafter &other);
  BlockAutoCrafter(BlockAutoCrafter &&other);
  /// �� ���������� ��������� ����������� � �����������.
  const BlockAutoCrafter &operator=(const BlockAutoCrafter &other) = delete;
  BlockAutoCrafter &operator=(BlockAutoCrafter &&other) = delete;

  /// ������������ ��� ������������.
  BlockAutoCrafter(const BlockAutoCrafter &other, Sector &parent);
  BlockAutoCrafter(BlockAutoCrafter &&other, Sector &parent);

  /// ������� ������� ����� �������.
  BlockAutoCrafter(const DataBase &db, const rapidjson::Value &val, Sector &parent, BlockId id);

  std::unique_ptr<Block> Clone(Sector &parent) override;

  void DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx, IContext & context) override;

  void Update(const Magnum::Timeline &dt) override;

  void Draw(const Magnum::Matrix4 &vp, Magnum::AbstractShaderProgram& shader) override;

private:
  Accessor &mCrafterInput;
  Accessor &mCrafterOutput;
  Accessor &mGeneratorInput;
  Accessor &mGeneratorOutput;

  Crafter mCrafter;
  Crafter mGenerator;

private:
  IRecipe::Tag CrafterType(const char *type, const DataBase & db, const rapidjson::Value &json) const;
  bool CrafterFast(const char *type, const DataBase & db, const rapidjson::Value &json) const;
  boost::optional<Accessor &> GetAccessor(const char *type, const char *dir, const rapidjson::Value &json) const;
};

#endif // BlockAutoCrafter_h__
