#include "Tesselator.h"



Tesselator::Tesselator(TesselatorType type)
  : mType(type)
{
}


Tesselator::~Tesselator()
{
}

Tesselator::TesselatorType Tesselator::Type() const
{
  return mType;
}
