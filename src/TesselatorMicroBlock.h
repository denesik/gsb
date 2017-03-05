#pragma once
#ifndef TesselatorMicroBlock_h__
#define TesselatorMicroBlock_h__


#include "Tesselator.h"
#include "tools/CoordSystem.h"
#include <vector>
#include "TesselatorSolidBlock.h"
#include <bitset>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Range.h>

typedef std::bitset<MICROBLOCK_CAPACITY> TessMicroBlockData;


class TesselatorMicroBlock : public Tesselator
{
public:
  typedef Magnum::Vector3i MBPos;
  typedef Magnum::Int MBPosType;

  TesselatorMicroBlock(size_t size = 4);
  ~TesselatorMicroBlock();

  TesselatorMicroBlock &SetTexture(const Magnum::Range2D &range, SideFlags side = ALL);

  void PushBack(const TesselatorData &microblock_data, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side = ALL) const;

  void JsonLoad(const rapidjson::Value & val, const TextureAtlas &atlas) override {};

  IndexType ToIndex(const MBPos &pos) const
  {
    const auto size = static_cast<IndexType>(mSize);
    return static_cast<IndexType>(pos.z()) * size * size +
      static_cast<IndexType>(pos.y()) * size +
      static_cast<IndexType>(pos.x());
  }

  MBPos FromIndex(IndexType i) const
  {
    const auto size = static_cast<IndexType>(mSize);
    return STPos{
      static_cast<STPosType>(i % size),
      static_cast<STPosType>((i / size) % size),
      static_cast<STPosType>(i / (size * size)) };
  }

  MBPosType Size() const
  {
    return mSize;
  }

  static TessMicroBlockData &ToMicroblockData(TesselatorData &data);

  static const TessMicroBlockData &ToMicroblockData(const TesselatorData &data);

private:
  std::vector<TesselatorSolidBlock> mData;
  Magnum::Range2D mTextureCoord[6];
  const MBPosType mSize;

private:
  Magnum::Range2D TextureCoord(const Magnum::Vector2i &pos) const
  {
    const auto len = 1.0f / Magnum::Float(mSize);
    return Magnum::Range2D({ Magnum::Float(pos.x()) * len, Magnum::Float(pos.y()) * len }, 
    { Magnum::Float(pos.x() + 1) * len, Magnum::Float(pos.y() + 1) * len });
  }
};


#endif // TesselatorMicroBlock_h__
