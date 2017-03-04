#include "TesselatorMicroBlock.h"

using namespace Magnum;


static inline Range2D TextureCoord(const Vector2i &pos)
{
  const auto len = 1.0f / Float(MICROBLOCK_SIZE);
  return Range2D( {Float(pos.x()) * len, Float(pos.y()) * len}, {Float(pos.x() + 1) * len, Float(pos.y() + 1) * len} );
}

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

TesselatorMicroBlock::TesselatorMicroBlock()
  : Tesselator(Tesselator::TesselatorType::MICRO_BLOCK)
{

}


TesselatorMicroBlock::~TesselatorMicroBlock()
{
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

  for (MBPosType z = 0; z < MICROBLOCK_SIZE; z++)
    for (MBPosType y = 0; y < MICROBLOCK_SIZE; y++)
      for (MBPosType x = 0; x < MICROBLOCK_SIZE; x++)
      {
        mData[ToIndex(MBPos{ x, y, z })].SetScale(1.0f / Float(MICROBLOCK_SIZE));
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ MICROBLOCK_SIZE - x - 1, y }), mTextureCoord[SideFlagIndex(SideFlags::FRONT)]), SideFlags::FRONT);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ x, y }), mTextureCoord[SideFlagIndex(SideFlags::BACK)]), SideFlags::BACK);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ MICROBLOCK_SIZE - z - 1, y }), mTextureCoord[SideFlagIndex(SideFlags::LEFT)]), SideFlags::LEFT);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ z, y }), mTextureCoord[SideFlagIndex(SideFlags::RIGHT)]), SideFlags::RIGHT);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ MICROBLOCK_SIZE - x - 1, z }), mTextureCoord[SideFlagIndex(SideFlags::TOP)]), SideFlags::TOP);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ MICROBLOCK_SIZE - x - 1, MICROBLOCK_SIZE - z - 1 }), mTextureCoord[SideFlagIndex(SideFlags::BOTTOM)]), SideFlags::BOTTOM);
      }

  return *this;
}

void TesselatorMicroBlock::PushBack(const TesselatorData &microblock_data, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side /*= SideFlags::ALL*/) const
{
  const auto scale = 1.0f / Float(MICROBLOCK_SIZE);

  const TessMicroBlockData &data = ToMicroblockData(microblock_data);

  for (IndexType i = 0; i < data.size(); ++i)
  {
    if (data[i])
    {
      mData[i].PushBack(vertex, index, last_index,
        pos + WPos(FromIndex(i)) *scale);
    }
  }
}

