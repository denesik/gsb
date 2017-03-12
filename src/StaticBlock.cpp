#include "StaticBlock.h"



StaticBlock::StaticBlock()
{
}


StaticBlock::~StaticBlock()
{
}

void StaticBlock::SetTesselator(std::unique_ptr<Tesselator> tesselator)
{
  mTesselator = std::move(tesselator);
}

const std::unique_ptr<Tesselator> & StaticBlock::GetTesselator() const
{
  return mTesselator;
}
