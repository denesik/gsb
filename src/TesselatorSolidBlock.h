#pragma once
#ifndef TesselatorSolidBlock_h__
#define TesselatorSolidBlock_h__

#include <Magnum/Math/Range.h>
#include <vector>
#include "Tesselator.h"
#include "tools/CoordSystem.h"
#include <array>
#include "tools/Crc32.h"

class TesselatorSolidBlock;
namespace
{
  static auto TesselatorSolidBlockLoaded = Tesselator::factory::Register<TesselatorSolidBlock>::add("TesselatorSolidBlock");
}

class TesselatorSolidBlock : public Tesselator
{
public:
  TesselatorSolidBlock();
  ~TesselatorSolidBlock() = default;

  TesselatorSolidBlock(const TesselatorSolidBlock &other) = delete;
  TesselatorSolidBlock(TesselatorSolidBlock &&other) = delete;

  TesselatorSolidBlock &operator=(const TesselatorSolidBlock &other) = delete;
  TesselatorSolidBlock &operator=(TesselatorSolidBlock &&other) = delete;

  constexpr static const char * name() { return "TesselatorSolidBlock"; }
  constexpr static uint32_t id() { return crc32(name()); }


  TesselatorSolidBlock &SetTexture(const Magnum::Range2D &range, SideFlags side = SideFlags::ALL);

  void PushBack(std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side = SideFlags::ALL) const;

  void JsonLoad(const rapidjson::Value& val, const TextureAtlas& atlas) override;

  bool UseTesselatorData() const override;

  void SetScale(Magnum::Float scale);

  void Process(SectorCompiler &compiler, const STPos &pos) override;

private:
  Magnum::Range2D mTextureCoord[6];
  std::array<Magnum::Vector3, 24> mVertexData;
};



#endif // TesselatorSolidBlock_h__
