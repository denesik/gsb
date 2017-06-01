#include "TesselatorTexturedBlock.h"

#include <Magnum/Math/Vector2.h>
#include "SectorCompiler.h"
#include "DataBase.h"

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

void TesselatorTexturedBlock::PushBack(const Tesselator::Data &tesselator_data, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side /*= SideFlags::ALL*/) const
{
  const auto &data = static_cast<const Data &>(tesselator_data);

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

void TesselatorTexturedBlock::Process(SectorCompiler &compiler, const STPos &pos)
{
  auto &vertex = compiler.GetVertexData();
  auto &index = compiler.GetIndexData();
  UnsignedInt last_index = vertex.size();
  auto data_index = cs::STtoTI(pos);
  const auto &microblock_data = compiler.TesselatorsData()[data_index];

  WPos wpos(cs::TItoSB(data_index));

  PushBack(microblock_data, vertex, index, last_index, wpos);
}
