#include "BlocksDataBase.h"
#include "TesselatorSolidBlock.h"
#include "TesselatorMicroBlock.h"

using namespace Magnum;

BlocksDataBase::BlocksDataBase(const TextureAtlas &atlas, const TextureAtlas &atlas_items)
  : mAtlas(atlas), mAtlasItems(atlas_items)
{

  
}


BlocksDataBase::~BlocksDataBase()
{
}

const std::unique_ptr<BlockStaticPart> & BlocksDataBase::GetBlockStaticPart(BlockId id) const
{
  return std::get<0>(mBlocks[id]);
}

void BlocksDataBase::ApplyLoader(std::unique_ptr<IDataBaseLoader> loader)
{
  loader->Load(mAtlas, *this);
}

std::optional<BlockId> BlocksDataBase::BlockIdFromName(const std::string &name) const
{
  auto it = mBlockNames.find(name);
  if (it != mBlockNames.end())
  {
    return{ it->second };
  }

  return{};
}

bool BlocksDataBase::AddBlock(const std::string &name, BlockId id, std::unique_ptr<BlockStaticPart> static_part, std::unique_ptr<BlockDynamicPart> dynamic_part)
{
  if (std::get<0>(mBlocks[id]) || BlockIdFromName(name))
    return false;

  std::get<0>(mBlocks[id]) = std::move(static_part);
  std::get<1>(mBlocks[id]) = std::move(dynamic_part);
  mBlockNames.emplace(name, id);
  return true;
}

void BlocksDataBase::AddRecipe(std::unique_ptr<IRecipe> move)
{
  auto& specified_recipes = mRecipes[move->Id()];

  specified_recipes.emplace_back(std::move(move));
  const IRecipe* recipe = specified_recipes[specified_recipes.size() - 1].get();

  for (const auto& inp : recipe->Components())
    mItemUsing[inp.id].push_back(recipe);

  for (const auto& inp : recipe->Results())
    mItemRecipe[inp.id].push_back(recipe);
}

void BlocksDataBase::AddItem(const std::string &name, ItemId id, std::unique_ptr<IItem> move)
{
  mItems[id] = std::move(move);
  mItemNames.emplace(name, id);
}

ItemId BlocksDataBase::ItemIdFromName(const std::string& name)
{
  return mItemNames[name];
}

const std::unique_ptr<IItem> & BlocksDataBase::GetItem(ItemId id) const
{
  return mItems[id];
}

std::tuple<BlockId, std::unique_ptr<BlockDynamicPart>> BlocksDataBase::CreateBlock(BlockId id) const
{
  const auto &dyn = std::get<1>(mBlocks[id]);
  return{ id , dyn ? dyn->Clone() : nullptr };
}

const TextureAtlas & BlocksDataBase::GetAtlasItems() const
{
  return mAtlasItems;
}

std::vector<const IRecipe *> BlocksDataBase::GetRecipes(const IRecipe & as_this, const std::vector<std::tuple<ItemId, size_t>> &items) const
{
  std::vector<const IRecipe *> result;

  // 1. Пробегаем по рецептам.
  // 2. Каждую компоненту рецепта ищем в списке итемов.
  // 3. Если компоненты нет -- рецепт не сработает.
  for (const auto &rec : GetSameRecipes(as_this))
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

const std::vector<std::unique_ptr<IRecipe>> & BlocksDataBase::GetSameRecipes(const IRecipe& as_this) const
{
  auto same = mRecipes.find(as_this.Id());
  if (same != mRecipes.end())
    return same->second;
  
  static const std::vector<std::unique_ptr<IRecipe>> null;

  return null;
}
