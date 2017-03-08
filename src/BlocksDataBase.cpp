#include "BlocksDataBase.h"
#include "TesselatorSolidBlock.h"
#include "TesselatorMicroBlock.h"

using namespace Magnum;

BlocksDataBase::BlocksDataBase(const TextureAtlas &atlas)
  : mAtlas(atlas)
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
  mRecipes.emplace_back(std::move(move));
  const IRecipe * recipe = mRecipes[mRecipes.size() - 1].get();

  for (const auto & inp : recipe->Components())
    mItemUsing[inp.id].push_back(recipe);

  for (const auto & inp : recipe->Results())
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

std::tuple<BlockId, std::unique_ptr<BlockDynamicPart>> BlocksDataBase::CreateBlock(BlockId id) const
{
  const auto &dyn = std::get<1>(mBlocks[id]);
  return{ id , dyn ? dyn->Clone() : nullptr };
}
