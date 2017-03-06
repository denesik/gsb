#include "BlocksDataBase.h"
#include "TesselatorSolidBlock.h"
#include "TesselatorMicroBlock.h"

using namespace Magnum;

BlocksDataBase::BlocksDataBase(const TextureAtlas &atlas)
  : mAtlas(atlas)
{

  {
    std::get<0>(mBlocks[1]) = std::make_unique<BlockStaticPart>();
    auto tesselator = std::make_unique<TesselatorSolidBlock>();
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/test_texture.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }));
    std::get<0>(mBlocks[1])->SetTesselator(std::move(tesselator));
  }

  {
    std::get<0>(mBlocks[2]) = std::make_unique<BlockStaticPart>();
    auto tesselator = std::make_unique<TesselatorSolidBlock>();
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::FRONT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::RIGHT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::BACK);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::LEFT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_top.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::TOP);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/dirt.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::BOTTOM);
    std::get<0>(mBlocks[2])->SetTesselator(std::move(tesselator));
  }

  {
    std::get<0>(mBlocks[3]) = std::make_unique<BlockStaticPart>();
    auto tesselator = std::make_unique<TesselatorMicroBlock>();
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::FRONT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::RIGHT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::BACK);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::LEFT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_top.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::TOP);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/dirt.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::BOTTOM);
    std::get<0>(mBlocks[3])->SetTesselator(std::move(tesselator));
    std::get<1>(mBlocks[3]) = std::make_unique<BlockDynamicPart>();
    std::get<1>(mBlocks[3])->mTesselatorData = std::make_unique<TesselatorData>();
  }
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

std::tuple<BlockId, std::unique_ptr<BlockDynamicPart>> BlocksDataBase::CreateBlock(BlockId id) const
{
  const auto &dyn = std::get<1>(mBlocks[id]);
  return{ id , dyn ? dyn->Clone() : nullptr };
}
