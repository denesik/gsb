#include "TesselatorSolidBlock.h"
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Vector2.h>
#include "TextureAtlas.h"
#include "CubeMeshData.h"

using namespace Magnum;




TesselatorSolidBlock::TesselatorSolidBlock()
  : Tesselator(Tesselator::TesselatorType::SOLID_BLOCK)
{
  //SetScale(1.0f);
}


TesselatorSolidBlock::~TesselatorSolidBlock()
{
}

TesselatorSolidBlock &TesselatorSolidBlock::SetTexture(const Magnum::Range2D &range, SideFlags side)
{
  for (int i = 0; i < 6; ++i)
  {
    if (side & (1 << i))
    {
      mTextureCoord[i] = range;
    }
  }
  return *this;
}

void TesselatorSolidBlock::PushBack(std::vector<TesselatorVertex> &vertex,
  std::vector<UnsignedInt> &index, UnsignedInt &last_index, const SBPos &pos, SideFlags side) const
{
  Vector3 bpos{ pos };

  for (int i = 0; i < 6; ++i)
  {
    if (side & (1 << i))
    {
      for (size_t j = 0; j < 4; ++j)
      {
        vertex.emplace_back(gVertexData[i * 4 + j] + bpos, Vector2{
          gTextureSide[j].x() * (mTextureCoord[i].right() - mTextureCoord[i].left()) + mTextureCoord[i].left(),
          gTextureSide[j].y() * (mTextureCoord[i].top() - mTextureCoord[i].bottom()) + mTextureCoord[i].bottom()
        });
      }

      for (size_t j = 0; j < 6; ++j)
      {
        index.push_back(gIndexSide[j] + last_index);
      }
      last_index += 4;
    }
  }
}

void TesselatorSolidBlock::JsonLoad(const rapidjson::Value& val, const TextureAtlas &atlas)
{
  if (val.HasMember("tex") && val["tex"].IsArray() && val["tex"].Size() == 6)
  {
    const rapidjson::Value &arr = val["tex"];
    SetTexture(atlas.GetTextureCoord(arr[0].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), BACK);
    SetTexture(atlas.GetTextureCoord(arr[1].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), FRONT);
    SetTexture(atlas.GetTextureCoord(arr[2].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), RIGHT);
    SetTexture(atlas.GetTextureCoord(arr[3].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), LEFT);
    SetTexture(atlas.GetTextureCoord(arr[4].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), TOP);
    SetTexture(atlas.GetTextureCoord(arr[5].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), BOTTOM);
  }
}

void TesselatorSolidBlock::SetScale(Float scale)
{
  for (size_t i = 0; i < mVertexData.size(); ++i)
  {
    mVertexData[i] = gVertexData[i]/* * scale*/;
  }
}
