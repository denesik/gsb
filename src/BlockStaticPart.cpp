#include "BlockStaticPart.h"



BlockStaticPart::BlockStaticPart()
{
}


BlockStaticPart::~BlockStaticPart()
{
}

void BlockStaticPart::SetTesselator(std::unique_ptr<Tesselator> tesselator)
{
  mTesselator = std::move(tesselator);
}

const std::unique_ptr<Tesselator> & BlockStaticPart::GetTesselator() const
{
  return mTesselator;
}
