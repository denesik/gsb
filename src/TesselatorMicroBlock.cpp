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
      b.bottom() * (b.top() - b.bottom()) + b.bottom(),
    },
    {
      a.right() * (b.right() - b.left()) + b.left(),
      b.top() * (b.top() - b.bottom()) + b.bottom()
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

TesselatorSolidBlock & TesselatorMicroBlock::SetTexture(const Magnum::Range2D &range, SideFlags side /*= SideFlags::ALL*/)
{
  mTextureCoord = range;

  for (MBPosType z = 0; z < MICROBLOCK_SIZE; z++)
    for (MBPosType y = 0; y < MICROBLOCK_SIZE; y++)
      for (MBPosType x = 0; x < MICROBLOCK_SIZE; x++)
      {
        mData[ToIndex(MBPos{ x, y, z })].SetScale(1.0f / Float(MICROBLOCK_SIZE));
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ x, y }), range), SideFlags::FRONT);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ x, y }), range), SideFlags::BACK);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ z, y }), range), SideFlags::LEFT);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ z, y }), range), SideFlags::RIGHT);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ x, z }), range), SideFlags::TOP);
        mData[ToIndex(MBPos{ x, y, z })].SetTexture(TextureCoordTo(TextureCoord({ x, z }), range), SideFlags::BOTTOM);
      }

}

void TesselatorMicroBlock::PushBack(std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const SBPos &pos, SideFlags side /*= SideFlags::ALL*/) const
{

}
