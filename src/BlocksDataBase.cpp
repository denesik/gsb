#include "BlocksDataBase.h"
#include "TesselatorSolidBlock.h"
#include "TesselatorMicroBlock.h"

using namespace Magnum;

BlocksDataBase::BlocksDataBase(const TextureAtlas &atlas)
  : mAtlas(atlas)
{
  mBlocks.reserve(0xFFFF);
  mBlocks.emplace_back();

  {
    mBlocks.emplace_back();
    mBlocks[1] = std::make_unique<BlockStaticPart>();
    auto tesselator = std::make_unique<TesselatorSolidBlock>();
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/test_texture.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }));
    mBlocks[1]->SetTesselator(std::move(tesselator));
  }

  {
    mBlocks.emplace_back();
    mBlocks[2] = std::make_unique<BlockStaticPart>();
    auto tesselator = std::make_unique<TesselatorSolidBlock>();
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::FRONT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::RIGHT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::BACK);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::LEFT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_top.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::TOP);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/dirt.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::BOTTOM);
    mBlocks[2]->SetTesselator(std::move(tesselator));
  }

  {
    mBlocks.emplace_back();
    mBlocks[3] = std::make_unique<BlockStaticPart>();
    auto tesselator = std::make_unique<TesselatorMicroBlock>(2);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::FRONT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::RIGHT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::BACK);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_side.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::LEFT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/grass_top.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::TOP);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/dirt.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), SideFlags::BOTTOM);
    mBlocks[3]->SetTesselator(std::move(tesselator));
  }
}


BlocksDataBase::~BlocksDataBase()
{
}

const std::unique_ptr<BlockStaticPart> & BlocksDataBase::GetBlockStaticPart(BlockId id) const
{
  return mBlocks[id];
}

void BlocksDataBase::ApplyLoader(std::unique_ptr<IDataBaseLoader> loader)
{
  loader->Load(mAtlas, mBlocks);
}
