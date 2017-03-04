#include "TesselatorMicroBlock.h"

using namespace Magnum;

typedef Magnum::Vector3i MBPos;
typedef Magnum::Int MBPosType;

static inline IndexType ToIndex(const MBPos &pos)
{
  const IndexType size = static_cast<IndexType>(MICROBLOCK_SIZE);
  return static_cast<IndexType>(pos.z()) * size * size +
    static_cast<IndexType>(pos.y()) * size +
    static_cast<IndexType>(pos.x());
}

static inline MBPos FromIndex(IndexType i)
{
  const IndexType size = static_cast<IndexType>(MICROBLOCK_SIZE);
  return STPos{
    static_cast<STPosType>(i % size),
    static_cast<STPosType>((i / size) % size),
    static_cast<STPosType>(i / (size * size)) };
}

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

void TesselatorMicroBlock::PushBack(std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side /*= SideFlags::ALL*/) const
{
  const auto scale = 1.0f / Float(MICROBLOCK_SIZE);

  for (MBPosType z = 0; z < MICROBLOCK_SIZE; z++)
    for (MBPosType y = 0; y < MICROBLOCK_SIZE; y++)
      for (MBPosType x = 0; x < MICROBLOCK_SIZE; x++)
      {
        mData[ToIndex(MBPos{ x, y, z })].PushBack(vertex, index, last_index,
          pos + WPos(static_cast<WPosType>(x), static_cast<WPosType>(y), static_cast<WPosType>(z)) *scale);
      }
// 
//   MBPosType x = 0;
//   MBPosType y = 0;
//   MBPosType z = 0;
//   mData[ToIndex(MBPos{ x, y, z })].PushBack(vertex, index, last_index, 
//     pos + WPos(static_cast<WPosType>(x), static_cast<WPosType>(y), static_cast<WPosType>(z)) *scale);
}
