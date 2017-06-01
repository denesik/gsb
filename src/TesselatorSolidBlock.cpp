#include "TesselatorSolidBlock.h"
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Vector2.h>
#include "TextureAtlas.h"
#include "tools/CoordSystem.h"
#include "SectorCompiler.h"
#include "DataBase.h"

using namespace Magnum;

enum VertexPos
{
  SWD, //FLD
  SED, //FRD
  SET, //FRT
  SWT, //FLT
  NWD, //BLD
  NED, //BRD
  NET, //BRT
  NWT, //BLT
};

static const Magnum::Vector3 gVertexCube[] =
{
  (WPos(cs::South(cs::West(cs::Down({})))) + WPos(1)) / 2.0f,
  (WPos(cs::South(cs::East(cs::Down({})))) + WPos(1)) / 2.0f,
  (WPos(cs::South(cs::East(cs::Top({})))) + WPos(1)) / 2.0f,
  (WPos(cs::South(cs::West(cs::Top({})))) + WPos(1)) / 2.0f,
  (WPos(cs::North(cs::West(cs::Down({})))) + WPos(1)) / 2.0f,
  (WPos(cs::North(cs::East(cs::Down({})))) + WPos(1)) / 2.0f,
  (WPos(cs::North(cs::East(cs::Top({})))) + WPos(1)) / 2.0f,
  (WPos(cs::North(cs::West(cs::Top({})))) + WPos(1)) / 2.0f,
};

static const Magnum::Vector3 gVertexData[] =
{
  gVertexCube[SWD],gVertexCube[SED],gVertexCube[SET],gVertexCube[SWT], // South
  gVertexCube[SED],gVertexCube[NED],gVertexCube[NET],gVertexCube[SET], // East
  gVertexCube[NED],gVertexCube[NWD],gVertexCube[NWT],gVertexCube[NET], // North
  gVertexCube[NWD],gVertexCube[SWD],gVertexCube[SWT],gVertexCube[NWT], // West
  gVertexCube[SWT],gVertexCube[SET],gVertexCube[NET],gVertexCube[NWT], // top
  gVertexCube[NWD],gVertexCube[NED],gVertexCube[SED],gVertexCube[SWD], // bot
};

static const Magnum::Vector3 gVertexNormal[] =
{
  { 0.f,  0.f,  1.f }, // front z+
  { -1.f,  0.f,  0.f }, // left x-
  { 0.f,  0.f, -1.f }, // back z-
  { 1.f,  0.f,  0.f }, // right x+
  { 0.f, -1.f,  0.f }, // top y-
  { 0.f,  1.f,  0.f }, // bot y+
};

static const Magnum::Vector2 gTextureSide[] =
{
  { 0.0f, 0.0f },
  { 1.0f, 0.0f },
  { 1.0f, 1.0f },
  { 0.0f, 1.0f },
};

static const Magnum::UnsignedInt gIndexSide[] =
{
  0, 1, 2, 2, 3, 0
};


TesselatorSolidBlock::TesselatorSolidBlock()
  : Tesselator(identity<TesselatorSolidBlock>())
{
  SetScale(1.0f);
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
        vertex.emplace_back(mVertexData[i * 4 + j] + pos, Vector2{
          gTextureSide[j].x() * (mTextureCoord[i].right() - mTextureCoord[i].left()) + mTextureCoord[i].left(),
          gTextureSide[j].y() * (mTextureCoord[i].top() - mTextureCoord[i].bottom()) + mTextureCoord[i].bottom()
        }, gVertexNormal[i]);
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
    SetTexture(atlas.GetTextureCoord(arr[0].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), SOUTH);
    SetTexture(atlas.GetTextureCoord(arr[1].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), EAST);
    SetTexture(atlas.GetTextureCoord(arr[2].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), NORTH);
    SetTexture(atlas.GetTextureCoord(arr[3].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), WEST);
    SetTexture(atlas.GetTextureCoord(arr[4].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), TOP);
    SetTexture(atlas.GetTextureCoord(arr[5].GetString()).value_or(Range2D{ Vector2{0.0f}, Vector2{1.0f} }), DOWN);
  }
}

void TesselatorSolidBlock::SetScale(Float scale)
{
  for (size_t i = 0; i < mVertexData.size(); ++i)
  {
    mVertexData[i] = gVertexData[i] * scale;
  }
}

void TesselatorSolidBlock::Process(SectorCompiler &compiler, const STPos &pos)
{
  const auto &db = compiler.GetDataBase();
  const auto &tesselators = compiler.Tesselators();

  // Рисовать сторону если тесселятор у смежного блока отсутствует или другого типа.
  int side = SideFlags::NONE;
  {
    const auto &block = db.GetBlockStaticPart(tesselators[cs::STtoTI(cs::West(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->id() != TesselatorSolidBlock::id())
      side |= SideFlags::WEST;
  }
  {
    const auto &block = db.GetBlockStaticPart(tesselators[cs::STtoTI(cs::East(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->id() != TesselatorSolidBlock::id())
      side |= SideFlags::EAST;
  }
  {
    const auto &block = db.GetBlockStaticPart(tesselators[cs::STtoTI(cs::Top(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->id() != TesselatorSolidBlock::id())
      side |= SideFlags::TOP;
  }
  {
    const auto &block = db.GetBlockStaticPart(tesselators[cs::STtoTI(cs::Down(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->id() != TesselatorSolidBlock::id())
      side |= SideFlags::DOWN;
  }
  {
    const auto &block = db.GetBlockStaticPart(tesselators[cs::STtoTI(cs::North(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->id() != TesselatorSolidBlock::id())
      side |= SideFlags::NORTH;
  }
  {
    const auto &block = db.GetBlockStaticPart(tesselators[cs::STtoTI(cs::South(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->id() != TesselatorSolidBlock::id())
      side |= SideFlags::SOUTH;
  }

  auto &vertex = compiler.GetVertexData();
  auto &index = compiler.GetIndexData();
  UnsignedInt last_index = vertex.size();

  WPos wpos(cs::TItoSB(cs::STtoTI(pos)));

  PushBack(vertex, index, last_index, wpos, static_cast<SideFlags>(side));
}

bool TesselatorSolidBlock::UseTesselatorData() const
{
  return false;
}
