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
#include <boost/icl/interval_map.hpp>

struct FakeData;

// TODO: �������� const std::unique_ptr<***> & 
// �� std::optional<const *** &>  ???
class DataBase
{
public:
  DataBase(const TextureAtlas &atlas, const TextureAtlas &atlas_items);
  ~DataBase();

  using ItemStorage = std::array<std::unique_ptr<IItem>, 0xFFFF>;
  using ItemStorageInterval = boost::icl::interval_map<unsigned short, bool>;

  /// �������� ���� � ��.
  /// ���� ���� � ��������� �� ����������, �� �� ����������� � ������������ false.
  bool AddBlock(const std::string &name, BlockId id, std::unique_ptr<StaticBlock> static_part, std::unique_ptr<Block> dynamic_part);

  void AddItem(const std::string &name, ItemId id, std::unique_ptr<IItem> move);

  void ApplyLoader(std::unique_ptr<IDataBaseLoader> loader);

  void AddRecipe(const std::string &tag, std::unique_ptr<IRecipe> move);

  const std::unique_ptr<StaticBlock> &GetBlockStaticPart(BlockId id) const;

  boost::optional<BlockId> BlockIdFromName(const std::string &name) const;

  /// ������� ����.
  /// ���� ���������� ������� ����������� �� ������ ���� �������.
  std::tuple<BlockId, std::unique_ptr<Tesselator::Data>,  std::unique_ptr<Block>> CreateBlock(Sector &parent, BlockId id) const;
  
  boost::optional<ItemId> ItemIdFromName(const std::string& name) const;

  const size_t GetItemCount() const;
  const std::unique_ptr<IItem> &GetItem(ItemId id) const;
  const ItemStorage &GetItems() const;
  const ItemStorageInterval &GetLoadedItemsInterval() const;


  const TextureAtlas &GetAtlasItems() const;
  const TextureAtlas &GetAtlas() const;

  ItemId NextItemId();

  /// �������� ������ �������� ��� ���������� ������ ������.
  std::vector<std::reference_wrapper<const IRecipe>> GetRecipes(IRecipe::Tag tag, const std::vector<std::tuple<ItemId, size_t>> &items) const;

  boost::optional<IRecipe::Tag> RecipeTagFromName(const std::string &name) const;

  //const std::vector<std::unique_ptr<IRecipe>> & GetSameRecipes(IRecipe::Tag tag) const;

  FakeData &GetFakeData();

private:
  std::array<std::tuple<std::unique_ptr<StaticBlock>, std::unique_ptr<Block>>, 0xFFFF> mBlocks;
  std::unordered_map<std::string, BlockId> mBlockNames;


  size_t mItemCount;
  ItemStorage mItems;
  ItemStorageInterval mLoadedItemsInterval;
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
  ItemId mLastItemId = 0xFFFF / 2; //TODO: ��������� ������� �� ���������
};



#endif // BlocksDataBase_h__
