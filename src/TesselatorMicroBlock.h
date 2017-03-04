#pragma once
#ifndef TesselatorMicroBlock_h__
#define TesselatorMicroBlock_h__


#include "Tesselator.h"
#include "tools/CoordSystem.h"
#include <array>
#include "TesselatorSolidBlock.h"


class TesselatorMicroBlock : public Tesselator
{
public:
  TesselatorMicroBlock();
  ~TesselatorMicroBlock();

  TesselatorMicroBlock &SetTexture(const Magnum::Range2D &range, SideFlags side = SideFlags::ALL);

  void PushBack(std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, SideFlags side = SideFlags::ALL) const;

  void JsonLoad(const rapidjson::Value & val, const TextureAtlas &atlas) override {};

private:
  std::array<TesselatorSolidBlock, MICROBLOCK_CAPACITY> mData;

  Magnum::Range2D mTextureCoord[6];

};



#endif // TesselatorMicroBlock_h__
