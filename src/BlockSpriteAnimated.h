#pragma once
#ifndef BlockSpriteAnimated_h__
#define BlockSpriteAnimated_h__

#include "Block.h"
#include "Movable.h"


class BlockSpriteAnimated;
namespace
{
  static auto BlockSpriteAnimatedLoaded = Block::factory::Register<BlockSpriteAnimated>::add("BlockSpriteAnimated");
}

/// Блок, текстура которого всегда повернута к игроку.
class BlockSpriteAnimated : public Block
{
public:
  BlockSpriteAnimated() = delete;
  virtual ~BlockSpriteAnimated() = default;

  BlockSpriteAnimated(const BlockSpriteAnimated &other);
  BlockSpriteAnimated(BlockSpriteAnimated &&other);
  /// Не используем операторы копирования и перемещения.
  const BlockSpriteAnimated &operator=(const BlockSpriteAnimated &other) = delete;
  BlockSpriteAnimated &operator=(BlockSpriteAnimated &&other) = delete;

  /// Конструкторы для клонирования.
  BlockSpriteAnimated(const BlockSpriteAnimated &other, Sector &parent);
  BlockSpriteAnimated(BlockSpriteAnimated &&other, Sector &parent);

  /// Создаем элемент через фабрику.
  BlockSpriteAnimated(const DataBase &db, const rapidjson::Value &val, Sector &parent, BlockId id);

  std::unique_ptr<Block> Clone(Sector &parent) override;

  void DrawGui(const Magnum::Timeline &dt) override;

  void Update(const Magnum::Timeline &dt) override;

  void Draw(const Magnum::Matrix4 &vp, Magnum::AbstractShaderProgram& shader) override;

private:
  size_t frame = 0;
  Movable mMovable;

};



#endif // BlockSpriteAnimated_h__
