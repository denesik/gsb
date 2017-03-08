#pragma once
#ifndef BlocksDataBase_h__
#define BlocksDataBase_h__

#include <vector>
#include <memory>
#include "BlockStaticPart.h"
#include "TextureAtlas.h"
#include "JsonDataBase.h"
#include <unordered_map>
#include <string>
#include <MagnumExternal/Optional/optional.hpp>
#include "BlockDynamicPart.h"
#include <array>

class BlocksDataBase
{
public:
  BlocksDataBase(const TextureAtlas &atlas, const TextureAtlas &atlas_items);
  ~BlocksDataBase();

  const std::unique_ptr<BlockStaticPart> &GetBlockStaticPart(BlockId id) const;

  void ApplyLoader(std::unique_ptr<IDataBaseLoader> loader);

  std::optional<BlockId> BlockIdFromName(const std::string &name) const;

  /// Добавить блок в БД.
  /// Если блок с указанным ид существует, он не добавляется и возвращается false.
  bool AddBlock(const std::string &name, BlockId id, std::unique_ptr<BlockStaticPart> static_part, std::unique_ptr<BlockDynamicPart> dynamic_part);

  /// Создает блок.
  /// Если невозможно создать блоксданным ид вернет блок воздуха.
  std::tuple<BlockId, std::unique_ptr<BlockDynamicPart>> CreateBlock(BlockId id) const;

  const TextureAtlas &GetAtlasItems() const;

private:
  std::array<std::tuple<std::unique_ptr<BlockStaticPart>, std::unique_ptr<BlockDynamicPart>>, 0xFFFF> mBlocks;
  std::unordered_map<BlockId, std::vector<std::unique_ptr<Agent>>> mAgents;
  std::unordered_map<std::string, BlockId> mBlockNames;

  const TextureAtlas &mAtlas;
  const TextureAtlas &mAtlasItems;
};



#endif // BlocksDataBase_h__
