#pragma once
#ifndef BlocksDataBase_h__
#define BlocksDataBase_h__

#include <vector>
#include <memory>
#include "StaticBlock.h"
#include "TextureAtlas.h"
#include "JsonDataBase.h"
#include <unordered_map>
#include <string>
#include <MagnumExternal/Optional/optional.hpp>
#include "Block.h"
#include <array>
#include "IRecipe.h"
#include "IItem.h"
#include <map>

// TODO: Заменить const std::unique_ptr<***> & 
// на std::optional<const *** &>  ???
class DataBase
{
public:
  DataBase(const TextureAtlas &atlas, const TextureAtlas &atlas_items);
  ~DataBase();

  const std::unique_ptr<StaticBlock> &GetBlockStaticPart(BlockId id) const;

  void ApplyLoader(std::unique_ptr<IDataBaseLoader> loader);

  std::optional<BlockId> BlockIdFromName(const std::string &name) const;

  /// Добавить блок в БД.
  /// Если блок с указанным ид существует, он не добавляется и возвращается false.
  bool AddBlock(const std::string &name, BlockId id, std::unique_ptr<StaticBlock> static_part, std::unique_ptr<Block> dynamic_part);

  /// Создает блок.
  /// Если невозможно создать блоксданным ид вернет блок воздуха.
  std::tuple<BlockId, std::unique_ptr<Block>> CreateBlock(BlockId id) const;

  void AddRecipe(std::unique_ptr<IRecipe> move);

  void AddItem(const std::string &name, ItemId id, std::unique_ptr<IItem> move);
  
  ItemId ItemIdFromName(const std::string& name);

  const std::unique_ptr<IItem> &GetItem(ItemId id) const;

  const TextureAtlas &GetAtlasItems() const;

  /// Получить список рецептов для указанного списка итемов.
  /// TODO: Переделать что б не было сырого указателя.
  /// Вероятно ид рецептов нужны.
  std::vector<const IRecipe *> GetRecipes(const IRecipe & as_this, const std::vector<std::tuple<ItemId, size_t>> &items) const;
  const std::vector<std::unique_ptr<IRecipe>> & GetSameRecipes(const IRecipe & as_this) const;

private:
  std::array<std::tuple<std::unique_ptr<StaticBlock>, std::unique_ptr<Block>>, 0xFFFF> mBlocks;
  std::unordered_map<std::string, BlockId> mBlockNames;

  std::array<std::unique_ptr<IItem>, 0xFFFF> mItems;
  std::unordered_map<std::string, ItemId> mItemNames;

  std::map<RecipeId, std::vector<std::unique_ptr<IRecipe>>> mRecipes;

  std::unordered_map<ItemId, std::vector<const IRecipe *>> mItemUsing;
  std::unordered_map<ItemId, std::vector<const IRecipe *>> mItemRecipe;

  const TextureAtlas &mAtlas;
  const TextureAtlas &mAtlasItems;
};



#endif // BlocksDataBase_h__
