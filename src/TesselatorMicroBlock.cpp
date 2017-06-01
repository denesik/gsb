#include "TesselatorMicroBlock.h"

using namespace Magnum;


static inline Range2D TextureCoordTo(const Range2D &a, const Range2D &b)
{
  return Range2D
  {
    {
      a.left() * (b.right() - b.left()) + b.left(),
      a.bottom() * (b.top() - b.bottom()) + b.bottom(),
    },
    {
      a.right() * (b.right() - b.left()) + b.left(),
      a.top() * (b.top() - b.bottom()) + b.bottom()
    }
  };
}

TessMicroBlockData & TesselatorMicroBlock::ToMicroblockData(TesselatorData &data)
{
  return reinterpret_cast<TessMicroBlockData &>(data);
}

const TessMicroBlockData & TesselatorMicroBlock::ToMicroblockData(const TesselatorData &data)
{
  return reinterpret_cast<const TessMicroBlockData &>(data);
}


TesselatorMicroBlock &TesselatorMicroBlock::Build()
{
  for (MBPosType z = 0; z < mSize; z++)
    for (MBPosType y = 0; y < mSize; y++)
      for (MBPosType x = 0; x < mSize; x++)
      {
        mData[ToIndex(MBPos{ x, y, z })].SetScale(1.0f / Float(mSize));
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ mSize - x - 1, y }), mTextureCoord[SideFlagIndex(SideFlags::SOUTH)]), SideFlags::SOUTH);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ x, y }), mTextureCoord[SideFlagIndex(SideFlags::NORTH)]), SideFlags::NORTH);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ mSize - z - 1, y }), mTextureCoord[SideFlagIndex(SideFlags::WEST)]), SideFlags::WEST);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ z, y }), mTextureCoord[SideFlagIndex(SideFlags::EAST)]), SideFlags::EAST);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ mSize - x - 1, z }), mTextureCoord[SideFlagIndex(SideFlags::TOP)]), SideFlags::TOP);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ mSize - x - 1, mSize - z - 1 }), mTextureCoord[SideFlagIndex(SideFlags::DOWN)]), SideFlags::DOWN);
      }
  return *this;
}

TesselatorMicroBlock::TesselatorMicroBlock(size_t size)
  : Tesselator(identity<TesselatorMicroBlock>())
{
  SetSize(size);
}


TesselatorMicroBlock & TesselatorMicroBlock::SetSize(size_t size)
{
  mSize = size;
  mData = decltype(mData)(mSize * mSize * mSize);
  return *this;
}

TesselatorMicroBlock & TesselatorMicroBlock::SetTexture(const Magnum::Range2D &range, SideFlags side /*= SideFlags::ALL*/)
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

void TesselatorMicroBlock::PushBack(const TesselatorData &microblock_data, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side /*= SideFlags::ALL*/) const
{
  const auto scale = 1.0f / Float(mSize);

  const TessMicroBlockData &data = ToMicroblockData(microblock_data);

  for (auto i = 0; i < mSize * mSize * mSize; ++i)
  {
    if (data[i])
    {
      mData[i].PushBack(vertex, index, last_index,
        pos + WPos(FromIndex(i)) *scale);
    }
  }
}

void TesselatorMicroBlock::JsonLoad(const rapidjson::Value & val, const TextureAtlas &atlas)
{
  if (val.HasMember("tex") && val["tex"].IsArray() && val["tex"].Size() == 6)
  {
    const rapidjson::Value &arr = val["tex"];
    SetTexture(atlas.GetTextureCoord(arr[0].GetString()).value_or(Range2D{ Vector2{ 0.0f }, Vector2{ 1.0f } }), SOUTH);
    SetTexture(atlas.GetTextureCoord(arr[1].GetString()).value_or(Range2D{ Vector2{ 0.0f }, Vector2{ 1.0f } }), EAST);
    SetTexture(atlas.GetTextureCoord(arr[2].GetString()).value_or(Range2D{ Vector2{ 0.0f }, Vector2{ 1.0f } }), NORTH);
    SetTexture(atlas.GetTextureCoord(arr[3].GetString()).value_or(Range2D{ Vector2{ 0.0f }, Vector2{ 1.0f } }), WEST);
    SetTexture(atlas.GetTextureCoord(arr[4].GetString()).value_or(Range2D{ Vector2{ 0.0f }, Vector2{ 1.0f } }), TOP);
    SetTexture(atlas.GetTextureCoord(arr[5].GetString()).value_or(Range2D{ Vector2{ 0.0f }, Vector2{ 1.0f } }), DOWN);
  }
  if (val.HasMember("size") && val["size"].IsUint())
  {
    SetSize(val["size"].GetInt());
  }
  Build();
}

bool TesselatorMicroBlock::UseTesselatorData() const
{
  return true;
}

