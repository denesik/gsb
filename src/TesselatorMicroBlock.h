#pragma once
#ifndef TesselatorMicroBlock_h__
#define TesselatorMicroBlock_h__


#include "Tesselator.h"
#include "tools/CoordSystem.h"
#include <array>
#include "TesselatorSolidBlock.h"
#include <bitset>
#include <Magnum/Magnum.h>

typedef std::bitset<MICROBLOCK_CAPACITY> TessMicroBlockData;


class TesselatorMicroBlock : public Tesselator
{
public:
  typedef Magnum::Vector3i MBPos;
  typedef Magnum::Int MBPosType;

  TesselatorMicroBlock();
  ~TesselatorMicroBlock();

  TesselatorMicroBlock &SetTexture(const Magnum::Range2D &range, SideFlags side = SideFlags::ALL);

  void PushBack(const TesselatorData &microblock_data, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side = SideFlags::ALL) const;

  void JsonLoad(const rapidjson::Value & val, const TextureAtlas &atlas) override {};

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

  static TessMicroBlockData &ToMicroblockData(TesselatorData &data);

  static const TessMicroBlockData &ToMicroblockData(const TesselatorData &data);

private:
  std::array<TesselatorSolidBlock, MICROBLOCK_CAPACITY> mData;
  Magnum::Range2D mTextureCoord[6];
};


#endif // TesselatorMicroBlock_h__
