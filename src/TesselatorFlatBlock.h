#pragma once
#ifndef TesselatorFlatBlock_h__
#define TesselatorFlatBlock_h__


#include "Tesselator.h"
#include "tools/CoordSystem.h"
#include <vector>
#include "TesselatorSolidBlock.h"
#include <bitset>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Range.h>
#include "IJsonSerializable.h"


class TesselatorFlatBlock;
namespace
{
  static auto TesselatorFlatBlockLoaded = Tesselator::factory::Register<TesselatorFlatBlock>::add("TesselatorFlatBlock");
}

class TesselatorFlatBlock : public Tesselator
{
public:
  struct Data : public Tesselator::Data
  {
    std::bitset<8 * 8 * 8> &get()
    {
      return reinterpret_cast<std::bitset<8 * 8 * 8> &>(data);
    }

    const std::bitset<8 * 8 * 8> &get() const
    {
      return reinterpret_cast<const std::bitset<8 * 8 * 8> &>(data);
    }
  };


  typedef Magnum::Vector3i MBPos;
  typedef Magnum::Int MBPosType;

  TesselatorFlatBlock(size_t size = 4);
  ~TesselatorFlatBlock() = default;

  TesselatorFlatBlock(const TesselatorFlatBlock &other) = delete;
  TesselatorFlatBlock(TesselatorFlatBlock &&other) = delete;

  TesselatorFlatBlock &operator=(const TesselatorFlatBlock &other) = delete;
  TesselatorFlatBlock &operator=(TesselatorFlatBlock &&other) = delete;

  constexpr static const char * name() { return "TesselatorFlatBlock"; }
  constexpr static uint32_t id() { return crc32(name()); }


  TesselatorFlatBlock &SetSize(size_t size);
  TesselatorFlatBlock &SetTexture(const Magnum::Range2D &range, SideFlags side = ALL);
  TesselatorFlatBlock &Build();

  void PushBack(const Tesselator::Data &microblock_data, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side = ALL) const;

  void JsonLoad(const rapidjson::Value & val, const TextureAtlas &atlas) override;

  bool UseTesselatorData() const override;

  IndexType ToIndex(const MBPos &pos) const
  {
    return ToIndex(pos, mSize);
  }

  MBPos FromIndex(IndexType i) const
  {
    return FromIndex(i, mSize);
  }

  static IndexType ToIndex(const MBPos &pos, IndexType mSize)
  {
    const auto size = static_cast<IndexType>(mSize);
    return static_cast<IndexType>(pos.z()) * size * size +
      static_cast<IndexType>(pos.y()) * size +
      static_cast<IndexType>(pos.x());
  }

  static MBPos FromIndex(IndexType i, IndexType mSize)
  {
    const auto size = static_cast<IndexType>(mSize);
    return STPos{
      static_cast<STPosType>(i % size),
      static_cast<STPosType>((i / size) % size),
      static_cast<STPosType>(i / (size * size)) 
    };
  }

  MBPosType Size() const
  {
    return mSize;
  }

private:
  std::vector<TesselatorSolidBlock> mData;
  Magnum::Range2D mTextureCoord[6];
  MBPosType mSize;

private:
  Magnum::Range2D TextureCoord(const Magnum::Vector2i &pos) const
  {
    const auto len = 1.0f / Magnum::Float(mSize);
    return Magnum::Range2D({ Magnum::Float(pos.x()) * len, Magnum::Float(pos.y()) * len }, 
    { Magnum::Float(pos.x() + 1) * len, Magnum::Float(pos.y() + 1) * len });
  }
};


#endif // TesselatorFlatBlock_h__
