#pragma once
#ifndef Tesselator_h__
#define Tesselator_h__

#include <Magnum/Magnum.h>
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

class Tesselator
{
public:
  enum class TesselatorType
  {
    SOLID_BLOCK,
  };

  Tesselator(TesselatorType type);
  virtual ~Tesselator();

  TesselatorType Type() const;

private:
  const TesselatorType mType;
};



#endif // Tesselator_h__
