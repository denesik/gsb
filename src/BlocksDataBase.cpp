#include "BlocksDataBase.h"
#include "TesselatorSolidBlock.h"

using namespace Magnum;

BlocksDataBase::BlocksDataBase(const TextureAtlas &atlas)
  : mAtlas(atlas)
{
  mBlocks.resize(0xFFFF);

  {
    mBlocks[1] = std::make_unique<BlockStaticPart>();
    auto tesselator = std::make_unique<TesselatorSolidBlock>();
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/test_texture.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }));
    mBlocks[1]->SetTesselator(std::move(tesselator));
  }

  {
    mBlocks[2] = std::make_unique<BlockStaticPart>();
    auto tesselator = std::make_unique<TesselatorSolidBlock>();
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/test_front.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), TesselatorSolidBlock::FRONT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/test_right.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), TesselatorSolidBlock::RIGHT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/test_back.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), TesselatorSolidBlock::BACK);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/test_left.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), TesselatorSolidBlock::LEFT);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/test_top.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), TesselatorSolidBlock::TOP);
    tesselator->SetTexture(mAtlas.GetTextureCoord("data/test_bottom.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }), TesselatorSolidBlock::BOTTOM);
    mBlocks[2]->SetTesselator(std::move(tesselator));
  }
}


BlocksDataBase::~BlocksDataBase()
{
}

const std::unique_ptr<BlockStaticPart> & BlocksDataBase::GetBlockStaticPart(BlockId id) const
{
  return mBlocks[id];
}
