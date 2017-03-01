#include "BlocksDataBase.h"
#include "TesselatorSolidBlock.h"

using namespace Magnum;

BlocksDataBase::BlocksDataBase(const TextureAtlas &atlas)
  : mAtlas(atlas)
{
  mBlocks.resize(0xFFFF);

  mBlocks[1] = std::make_unique<BlockStaticPart>();
  auto tesselator = std::make_unique<TesselatorSolidBlock>();
  tesselator->SetTexture(mAtlas.GetTextureCoord("data/test_texture.tga").value_or(Range2D{ Vector2{ 0.0f },Vector2{ 1.0f } }));
  mBlocks[1]->SetTesselator(std::move(tesselator));
}


BlocksDataBase::~BlocksDataBase()
{
}

const std::unique_ptr<BlockStaticPart> & BlocksDataBase::GetBlockStaticPart(BlockId id) const
{
  return mBlocks[id];
}
