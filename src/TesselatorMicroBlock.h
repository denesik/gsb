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

  TesselatorSolidBlock &SetTexture(const Magnum::Range2D &range, SideFlags side = SideFlags::ALL);

  void PushBack(std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const SBPos &pos, SideFlags side = SideFlags::ALL) const;

private:
  std::array<TesselatorSolidBlock, MICROBLOCK_SIZE> mData;

  Magnum::Range2D mTextureCoord;

};



#endif // TesselatorMicroBlock_h__
