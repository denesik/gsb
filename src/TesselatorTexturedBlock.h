#pragma once
#ifndef TesselatorTexturedBlock_h__
#define TesselatorTexturedBlock_h__

#include "Tesselator.h"
#include <bitset>
#include <stdint.h>
#include <Magnum\Magnum.h>
#include <Magnum/Math/Range.h>
#include <vector>
#include <array>
#include "TesselatorSolidBlock.h"

class TesselatorTexturedBlock;
namespace
{
  static auto TesselatorTexturedBlockLoaded = Tesselator::factory::Register<TesselatorTexturedBlock>::add("TesselatorTexturedBlock");
}

struct TessTexturedBlockData
{
  uint8_t sides[6] = {0, 0, 0, 0, 0, 0};
  uint16_t mirror = 0;

  void set_texture(size_t side, uint8_t index)
  {
    sides[side] |= (index & FLAG_TEXTURE);
  }

  uint8_t texture(size_t side) const
  {
    return sides[side] & FLAG_TEXTURE;
  }

private:
  enum 
  {
    FLAG_TEXTURE = 0x3F,
    FLAG_ROT_X = 0x40,
    FLAG_ROT_Y = 0x80,
  };
};

class TesselatorTexturedBlock : public Tesselator
{
public:
  TesselatorTexturedBlock();
  ~TesselatorTexturedBlock();

  TesselatorTexturedBlock &AddTexture(const Magnum::Range2D &range);

  void PushBack(const TesselatorData &tesselator_data, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side = SideFlags::ALL) const;

  bool UseTesselatorData() const override;

  static TessTexturedBlockData &ToTexturedBlockData(TesselatorData &data);

  static const TessTexturedBlockData &ToTexturedBlockData(const TesselatorData &data);

  void JsonLoad(const rapidjson::Value& val, const TextureAtlas& atlas) override {};

private:
  std::vector<Magnum::Range2D> mTextures;
  mutable TesselatorSolidBlock m_tesselator;

};



#endif // TesselatorTexturedBlock_h__
