#pragma once
#ifndef BlockResourceGenerator_h__
#define BlockResourceGenerator_h__

#include "Block.h"
#include "Timer.h"
#include "IRecipe.h"
#include "Crafter.h"
#include "accessors\AccessorHeat.h"
#include "Generator.h"

class BlockResourceGenerator;
namespace
{
  static auto BlockResourceGeneratorLoaded = Block::factory::Register<BlockResourceGenerator>::add("BlockResourceGenerator");
}

class BlockResourceGenerator : public Block
{
public:
  BlockResourceGenerator() = delete;
  virtual ~BlockResourceGenerator() = default;

  BlockResourceGenerator(const BlockResourceGenerator &other);
  BlockResourceGenerator(BlockResourceGenerator &&other);
  /// Не используем операторы копирования и перемещения.
  const BlockResourceGenerator &operator=(const BlockResourceGenerator &other) = delete;
  BlockResourceGenerator &operator=(BlockResourceGenerator &&other) = delete;

  /// Конструкторы для клонирования.
  BlockResourceGenerator(const BlockResourceGenerator &other, Sector &parent);
  BlockResourceGenerator(BlockResourceGenerator &&other, Sector &parent);

  /// Создаем элемент через фабрику.
  BlockResourceGenerator(const DataBase &db, const rapidjson::Value &val, Sector &parent, BlockId id);

  std::unique_ptr<Block> Clone(Sector &parent) override;

  void DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx, IContext & context) override;

  void Update(const Magnum::Timeline &dt) override;

  void Draw(const Magnum::Matrix4 &vp, Magnum::AbstractShaderProgram& shader) override;

private:
  Accessor &mGeneratorInput;
  Accessor &mGeneratorOutput;

  Crafter  mGenerator;

private:
  IRecipe::Tag CrafterType(const char *type, const DataBase & db, const rapidjson::Value &json) const;
  bool CrafterFast(const char *type, const DataBase & db, const rapidjson::Value &json) const;
  boost::optional<Accessor &> GetAccessor(const char *type, const char *dir, const rapidjson::Value &json) const;
};

#endif // BlockResourceGenerator_h__
