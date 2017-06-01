#include "TesselatorTexturedBlock.h"

#include <Magnum/Math/Vector2.h>
using namespace Magnum;

TesselatorTexturedBlock::TesselatorTexturedBlock()
  : Tesselator(identity<TesselatorTexturedBlock>())
{
}




TesselatorTexturedBlock & TesselatorTexturedBlock::AddTexture(const Magnum::Range2D &range)
{
  mTextures.emplace_back(range);
  return *this;
}

void TesselatorTexturedBlock::PushBack(const TesselatorData &tesselator_data, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side /*= SideFlags::ALL*/) const
{
  const TessTexturedBlockData &data = ToTexturedBlockData(tesselator_data);

  for (int i = 0; i < 6; ++i)
  {
    m_tesselator.SetTexture(mTextures[data.texture(i)], SideFlags(1 << i));
  }

  m_tesselator.PushBack(vertex, index, last_index, pos, side);
}

bool TesselatorTexturedBlock::UseTesselatorData() const
{
  return true;
}

TessTexturedBlockData & TesselatorTexturedBlock::ToTexturedBlockData(TesselatorData &data)
{
  return reinterpret_cast<TessTexturedBlockData &>(data);
}

const TessTexturedBlockData & TesselatorTexturedBlock::ToTexturedBlockData(const TesselatorData &data)
{
  return reinterpret_cast<const TessTexturedBlockData &>(data);
}

void TesselatorTexturedBlock::JsonLoad(const rapidjson::Value& val, const TextureAtlas& atlas)
{
  if (val.HasMember("tex") && val["tex"].IsArray())
  {
    const rapidjson::Value &arr = val["tex"];
    for (size_t i = 0; i < arr.Size(); ++i)
    {
      AddTexture(atlas.GetTextureCoord(arr[i].GetString()).value_or(Range2D{ Vector2{ 0.0f }, Vector2{ 1.0f } }));
    }
  }
}
