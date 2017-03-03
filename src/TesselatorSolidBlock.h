#pragma once
#ifndef TesselatorSolidBlock_h__
#define TesselatorSolidBlock_h__

#include <Magnum/Math/Range.h>
#include <vector>
#include "Tesselator.h"
#include "tools/CoordSystem.h"


class TesselatorSolidBlock : public Tesselator
{
public:
  TesselatorSolidBlock();
  ~TesselatorSolidBlock();

  TesselatorSolidBlock &SetTexture(const Magnum::Range2D &range, SideFlags side = SideFlags::ALL);

  void PushBack(std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const SBPos &pos, SideFlags side = SideFlags::ALL) const;

private:
  Magnum::Range2D mTextureCoord[6];
};



#endif // TesselatorSolidBlock_h__
