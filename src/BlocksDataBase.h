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

class BlocksDataBase
{
public:
  BlocksDataBase(const TextureAtlas &atlas);
  ~BlocksDataBase();

  const std::unique_ptr<BlockStaticPart> &GetBlockStaticPart(BlockId id) const;

  void ApplyLoader(std::unique_ptr<IDataBaseLoader> loader);

  std::optional<BlockId> BlockIdFromName(const std::string &name) const;

  /// Добавить блок в БД.
  /// Если блок с указанным ид существует, он не добавляется и возвращается false.
  bool AddBlock(const std::string &name, BlockId id, std::unique_ptr<BlockStaticPart> static_part);

private:
  std::vector<std::unique_ptr<BlockStaticPart>> mBlocks;
  std::unordered_map<std::string, BlockId> mBlockNames;

  const TextureAtlas &mAtlas;
};



#endif // BlocksDataBase_h__
