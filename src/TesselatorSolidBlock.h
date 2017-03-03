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
  enum Side : int 
  {
    NONE = 0,

    FRONT = 1 << 0,
    RIGHT = 1 << 1,
    BACK = 1 << 2,
    LEFT = 1 << 3,
    TOP = 1 << 4,
    BOTTOM = 1 << 5,

    ALL = 0x3F,
  };

  TesselatorSolidBlock();
  ~TesselatorSolidBlock();

  TesselatorSolidBlock &SetTexture(const Magnum::Range2D &range, Side side = Side::ALL);

  void PushBack(std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const SBPos &pos, Side side = Side::ALL) const;

private:
  Magnum::Range2D mTextureCoord[6];
};



#endif // TesselatorSolidBlock_h__
