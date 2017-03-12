#include "TesselatorSolidBlock.h"
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Vector2.h>
#include "TextureAtlas.h"

using namespace Magnum;

enum VertexPos
{
  FLB,
  FRB,
  FRT,
  FLT,
  BLB,
  BRB,
  BRT,
  BLT,
};

static const Magnum::Vector3 gVertexCube[] =
{
  { 1.0f, 0.0f, 0.0f },// FRONT LB
  { 0.0f, 0.0f, 0.0f },// FRONT RB
  { 0.0f, 1.0f, 0.0f },// FRONT RT
  { 1.0f, 1.0f, 0.0f },// FRONT LT
  { 1.0f, 0.0f, 1.0f },// BACK LB
  { 0.0f, 0.0f, 1.0f },// BACK RB
  { 0.0f, 1.0f, 1.0f },// BACK RT
  { 1.0f, 1.0f, 1.0f },// BACK LT
};

static const Magnum::Vector3 gNormalData[] =
{
  {  0,  0, -1 }, // front
  { -1,  0,  0 }, // right +
  {  0,  0,  1 }, // back +
  {  1,  0,  0 }, // left  
  {  0,  1,  0 }, // top
  {  0, -1,  0 }, // bot
};

static const Magnum::Vector3 gVertexData[] =
{
  gVertexCube[FLB],gVertexCube[FRB],gVertexCube[FRT],gVertexCube[FLT], // front
  gVertexCube[FRB],gVertexCube[BRB],gVertexCube[BRT],gVertexCube[FRT], // right +
  gVertexCube[BRB],gVertexCube[BLB],gVertexCube[BLT],gVertexCube[BRT], // back +
  gVertexCube[BLB],gVertexCube[FLB],gVertexCube[FLT],gVertexCube[BLT], // left
  gVertexCube[FLT],gVertexCube[FRT],gVertexCube[BRT],gVertexCube[BLT], // top
  gVertexCube[BLB],gVertexCube[BRB],gVertexCube[FRB],gVertexCube[FLB], // bot
};


static const Magnum::Vector2 gTextureSide[] =
{
  { 0.0f, 0.0f },
  { 1.0f, 0.0f },
  { 1.0f, 1.0f },
  { 0.0f, 1.0f },
};

static Magnum::UnsignedInt gIndexSide[] =
{
  0, 1, 2, 2, 3, 0
};


TesselatorSolidBlock::TesselatorSolidBlock()
  : Tesselator(Tesselator::TesselatorType::SOLID_BLOCK)
{
  SetScale(1.0f);
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
  std::vector<UnsignedInt> &index, UnsignedInt &last_index, const WPos &pos, SideFlags side) const
{
  for (int i = 0; i < 6; ++i)
  {
    if (side & (1 << i))
    {
      for (size_t j = 0; j < 4; ++j)
      {
        vertex.emplace_back(
          mVertexData[i * 4 + j] + pos,
          Vector2{
          gTextureSide[j].x() * (mTextureCoord[i].right() - mTextureCoord[i].left()) + mTextureCoord[i].left(),
          gTextureSide[j].y() * (mTextureCoord[i].top() - mTextureCoord[i].bottom()) + mTextureCoord[i].bottom()
        },
          gNormalData[i]);
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
    mVertexData[i] = gVertexData[i] * scale;
  }
}

bool TesselatorSolidBlock::UseTesselatorData() const
{
  return false;
}
