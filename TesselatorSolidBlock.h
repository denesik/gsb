#pragma once
#ifndef TesselatorSolidBlock_h__
#define TesselatorSolidBlock_h__

#include <Magnum/Math/Range.h>
#include <Magnum/Magnum.h>
#include <vector>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Vector2.h>


struct TesselatorVertex
{
  TesselatorVertex() = default;

  TesselatorVertex(const Magnum::Vector3 &_vertex, const Magnum::Vector2 &_textcoord)
    : vertex(_vertex), textcoord(_textcoord)
  {}

  Magnum::Vector3 vertex;
  Magnum::Vector2 textcoord;
};

class TesselatorSolidBlock
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

  void SetTexture(const Magnum::Range2D &range);

  void PushBack(std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, Side side = Side::ALL) const;

private:
  Magnum::Range2D mTextureCoord;
};



#endif // TesselatorSolidBlock_h__
