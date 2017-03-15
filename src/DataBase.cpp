#include "DataBase.h"
#include "TesselatorSolidBlock.h"
#include "TesselatorMicroBlock.h"
#include "World.h"
#include "Sector.h"
#include "DataBaseFakeData.h"

using namespace Magnum;

DataBase::DataBase(const TextureAtlas &atlas, const TextureAtlas &atlas_items)
  : mAtlas(atlas), mAtlasItems(atlas_items)
{
  mFakeData = std::make_unique<FakeData>(*this);
  
}


DataBase::~DataBase()
{
}

const std::unique_ptr<StaticBlock> & DataBase::GetBlockStaticPart(BlockId id) const
{
  return std::get<0>(mBlocks[id]);
}

void DataBase::ApplyLoader(std::unique_ptr<IDataBaseLoader> loader)
{
  loader->Load(mAtlas, *this);
}

boost::optional<BlockId> DataBase::BlockIdFromName(const std::string &name) const
{
  auto it = mBlockNames.find(name);
  if (it != mBlockNames.end())
  {
    return{ it->second };
  }

  return{};
}

bool DataBase::AddBlock(const std::string &name, BlockId id, std::unique_ptr<StaticBlock> static_part, std::unique_ptr<Block> dynamic_part)
{
  if (std::get<0>(mBlocks[id]) || BlockIdFromName(name))
    return false;

  std::get<0>(mBlocks[id]) = std::move(static_part);
  std::get<1>(mBlocks[id]) = std::move(dynamic_part);
  mBlockNames.emplace(name, id);
  return true;
}

void DataBase::AddRecipe(const std::string &tag_name, std::unique_ptr<IRecipe> move)
{
  auto tag = RecipeTagFromName(tag_name);

  if (!tag)
  {
    tag = mLastRecipeTag++;
    mRecipes.emplace(*tag, decltype(mRecipes)::mapped_type());
    mRecipeTags.emplace(tag_name, *tag);
  }

  auto& specified_recipes = mRecipes[*tag];

  specified_recipes.emplace_back(std::move(move));
  const IRecipe* recipe = specified_recipes[specified_recipes.size() - 1].get();

  for (const auto& inp : recipe->Components())
    mItemUsing[inp.id].push_back(recipe);

  for (const auto& inp : recipe->Results())
    mItemRecipe[inp.id].push_back(recipe);
}

void DataBase::AddItem(const std::string &name, ItemId id, std::unique_ptr<IItem> move)
{
  mItems[id] = std::move(move);
  mItemNames.emplace(name, id);
}

boost::optional<ItemId> DataBase::ItemIdFromName(const std::string& name) const
{
  auto it = mItemNames.find(name);
  if (it != mItemNames.end())
  {
    return{ it->second };
  }

  return{};
}

const std::unique_ptr<IItem> & DataBase::GetItem(ItemId id) const
{
  return mItems[id];
}

std::tuple<BlockId, std::unique_ptr<TesselatorData>, std::unique_ptr<Block>> DataBase::CreateBlock(Sector &parent, BlockId id) const
{
  std::unique_ptr<TesselatorData> tess_data;
  std::unique_ptr<Block> block;

  const auto &static_block = std::get<0>(mBlocks[id]);
  if (static_block)
  {
    const auto &dyn = std::get<1>(mBlocks[id]);
    const auto &tesselator = static_block->GetTesselator();

    tess_data = (tesselator && tesselator->UseTesselatorData()) ? std::make_unique<TesselatorData>() : nullptr;
    block = dyn ? dyn->Clone(parent) : nullptr;
  }

  return{ id , std::move(tess_data), std::move(block) };
}

const TextureAtlas & DataBase::GetAtlasItems() const
{
  return mAtlasItems;
}

std::vector<const IRecipe *> DataBase::GetRecipes(IRecipe::Tag tag, const std::vector<std::tuple<ItemId, size_t>> &items) const
{
  std::vector<const IRecipe *> result;

  // 1. Пробегаем по рецептам.
  // 2. Каждую компоненту рецепта ищем в списке итемов.
  // 3. Если компоненты нет -- рецепт не сработает.
  for (const auto &rec : GetSameRecipes(tag))
  {
    const auto &components = rec->Components();
    bool rec_found = true;
    for (const auto &c : components)
    {
      bool comp_found = false;
      for (auto i : items)
      {
        if (c.id == std::get<0>(i))
        {
          comp_found = true;
          break;
        }
      }
      if (!comp_found)
      {
        rec_found = false;
        break;
      }
    }

    if (rec_found)
    {
      result.emplace_back(rec.get());
    }
  }

  return result;
}

boost::optional<IRecipe::Tag> DataBase::RecipeTagFromName(const std::string &name) const
{
  auto it = mRecipeTags.find(name);
  if (it != mRecipeTags.end())
  {
    return{ it->second };
  }

  return{};
}

const std::vector<std::unique_ptr<IRecipe>> & DataBase::GetSameRecipes(IRecipe::Tag tag) const
{
  static const std::vector<std::unique_ptr<IRecipe>> null;

  auto same = mRecipes.find(tag);
  return same == mRecipes.end() ? null : same->second;
}

FakeData & DataBase::GetFakeData()
{
  return *mFakeData;
}

