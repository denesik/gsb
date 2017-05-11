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
#include "Block.h"
#include <array>
#include "IRecipe.h"
#include "IItem.h"
#include <map>
#include <boost\optional\optional.hpp>
#include <type_traits>

struct FakeData;

// TODO: Заменить const std::unique_ptr<***> & 
// на std::optional<const *** &>  ???
class DataBase
{
public:
  DataBase(const TextureAtlas &atlas, const TextureAtlas &atlas_items);
  ~DataBase();

  /// Добавить блок в БД.
  /// Если блок с указанным ид существует, он не добавляется и возвращается false.
  bool AddBlock(const std::string &name, BlockId id, std::unique_ptr<StaticBlock> static_part, std::unique_ptr<Block> dynamic_part);

  void AddItem(const std::string &name, ItemId id, std::unique_ptr<IItem> move);

  void ApplyLoader(std::unique_ptr<IDataBaseLoader> loader);

  void AddRecipe(const std::string &tag, std::unique_ptr<IRecipe> move);

  const std::unique_ptr<StaticBlock> &GetBlockStaticPart(BlockId id) const;

  boost::optional<BlockId> BlockIdFromName(const std::string &name) const;

  /// Создает блок.
  /// Если невозможно создать блоксданным ид вернет блок воздуха.
  std::tuple<BlockId, std::unique_ptr<TesselatorData>,  std::unique_ptr<Block>> CreateBlock(Sector &parent, BlockId id) const;
  
  boost::optional<ItemId> ItemIdFromName(const std::string& name) const;

  const std::unique_ptr<IItem> &GetItem(ItemId id) const;

  const TextureAtlas &GetAtlasItems() const;
  const TextureAtlas &GetAtlas() const;

  /// Получить список рецептов для указанного списка итемов.
  std::vector<std::reference_wrapper<const IRecipe>> GetRecipes(IRecipe::Tag tag, const std::vector<std::tuple<ItemId, size_t>> &items) const;

  boost::optional<IRecipe::Tag> RecipeTagFromName(const std::string &name) const;

  //const std::vector<std::unique_ptr<IRecipe>> & GetSameRecipes(IRecipe::Tag tag) const;

  FakeData &GetFakeData();

private:
  std::array<std::tuple<std::unique_ptr<StaticBlock>, std::unique_ptr<Block>>, 0xFFFF> mBlocks;
  std::unordered_map<std::string, BlockId> mBlockNames;

  std::array<std::unique_ptr<IItem>, 0xFFFF> mItems;
  std::unordered_map<std::string, ItemId> mItemNames;

  std::map<IRecipe::Tag, std::vector<std::unique_ptr<IRecipe>>> mRecipes;
  std::unordered_map<std::string, IRecipe::Tag> mRecipeTags;

  std::unordered_map<ItemId, std::vector<const IRecipe *>> mItemUsing;
  std::unordered_map<ItemId, std::vector<const IRecipe *>> mItemRecipe;

  const TextureAtlas &mAtlas;
  const TextureAtlas &mAtlasItems;

  IRecipe::Tag mLastRecipeTag = 0;

private:
  std::unique_ptr<FakeData> mFakeData;

};



#endif // BlocksDataBase_h__
