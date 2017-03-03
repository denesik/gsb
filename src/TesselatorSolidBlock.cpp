#include "TesselatorSolidBlock.h"
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Vector2.h>

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

static const Vector3 gVertexCube[] = 
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

static const Vector3 gVertexData[] =
{
  gVertexCube[FLB],gVertexCube[FRB],gVertexCube[FRT],gVertexCube[FLT], // front
  gVertexCube[FRB],gVertexCube[BRB],gVertexCube[BRT],gVertexCube[FRT], // right
  gVertexCube[BRB],gVertexCube[BLB],gVertexCube[BLT],gVertexCube[BRT], // back
  gVertexCube[BLB],gVertexCube[FLB],gVertexCube[FLT],gVertexCube[BLT], // left
  gVertexCube[FLT],gVertexCube[FRT],gVertexCube[BRT],gVertexCube[BLT], // top
  gVertexCube[BLB],gVertexCube[BRB],gVertexCube[FRB],gVertexCube[FLB], // bot
};

static const Vector2 gTextureSide[] =
{
  { 0.0f, 0.0f },
  { 1.0f, 0.0f }, 
  { 1.0f, 1.0f },
  { 0.0f, 1.0f },
};

static UnsignedInt gIndexSide[] =
{
  0, 1, 2, 2, 3, 0
};


TesselatorSolidBlock::TesselatorSolidBlock()
  : Tesselator(Tesselator::TesselatorType::SOLID_BLOCK)
{
}


TesselatorSolidBlock::~TesselatorSolidBlock()
{
}

TesselatorSolidBlock &TesselatorSolidBlock::SetTexture(const Magnum::Range2D &range, Side side)
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
  std::vector<UnsignedInt> &index, UnsignedInt &last_index, const SBPos &pos, Side side) const
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
