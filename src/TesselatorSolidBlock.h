#pragma once
#ifndef TesselatorSolidBlock_h__
#define TesselatorSolidBlock_h__

#include <Magnum/Math/Range.h>
#include <vector>
#include "Tesselator.h"
#include "tools/CoordSystem.h"
#include <array>


class TesselatorSolidBlock : public Tesselator
{
public:
  TesselatorSolidBlock();
  ~TesselatorSolidBlock();

  TesselatorSolidBlock &SetTexture(const Magnum::Range2D &range, SideFlags side = SideFlags::ALL);

  void PushBack(std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side = SideFlags::ALL) const;


  void JsonLoad(const rapidjson::Value& val, const TextureAtlas& atlas) override;

  void SetScale(Magnum::Float scale);

  bool UseTesselatorData() const override;

private:
  Magnum::Range2D mTextureCoord[6];
  std::array<Magnum::Vector3, 24> mVertexData;
};

REGISTER_TESSELLATOR(TesselatorSolidBlock)


#endif // TesselatorSolidBlock_h__
