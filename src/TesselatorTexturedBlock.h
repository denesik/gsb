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

class TesselatorTexturedBlock : public Tesselator
{
public:
  struct Data : public Tesselator::Data
  {
    struct data_type
    {
      uint8_t sides[6] = { 0, 0, 0, 0, 0, 0 };
      uint16_t mirror = 0;
    };

    void set_texture(size_t side, uint8_t index)
    {
      get().sides[side] |= (index & FLAG_TEXTURE);
    }

    uint8_t texture(size_t side) const
    {
      return get().sides[side] & FLAG_TEXTURE;
    }

    data_type &get()
    {
      return reinterpret_cast<data_type &>(data);
    }

    const data_type &get() const
    {
      return reinterpret_cast<const data_type &>(data);
    }

  private:
    enum
    {
      FLAG_TEXTURE = 0x3F,
      FLAG_ROT_X = 0x40,
      FLAG_ROT_Y = 0x80,
    };
  };

  TesselatorTexturedBlock();
  ~TesselatorTexturedBlock() = default;

  TesselatorTexturedBlock(const TesselatorTexturedBlock &other) = delete;
  TesselatorTexturedBlock(TesselatorTexturedBlock &&other) = delete;

  TesselatorTexturedBlock &operator=(const TesselatorTexturedBlock &other) = delete;
  TesselatorTexturedBlock &operator=(TesselatorTexturedBlock &&other) = delete;

  constexpr static const char * name() { return "TesselatorTexturedBlock"; }
  constexpr static uint32_t id() { return crc32(name()); }


  TesselatorTexturedBlock &AddTexture(const Magnum::Range2D &range);

  void PushBack(const Tesselator::Data &tesselator_data, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side = SideFlags::ALL) const;

  bool UseTesselatorData() const override;

  void JsonLoad(const rapidjson::Value& val, const TextureAtlas& atlas) override;

  void Process(SectorCompiler &compiler, const STPos &pos) override;

private:
  std::vector<Magnum::Range2D> mTextures;
  mutable TesselatorSolidBlock m_tesselator;

};



#endif // TesselatorTexturedBlock_h__
