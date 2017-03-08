#include "BlockDynamicPart.h"



BlockDynamicPart::BlockDynamicPart()
{
}


BlockDynamicPart::~BlockDynamicPart()
{
}

std::unique_ptr<BlockDynamicPart> BlockDynamicPart::Clone()
{
  auto part = std::make_unique<BlockDynamicPart>();
  if (mTesselatorData) part->mTesselatorData = std::make_unique<TesselatorData>(*mTesselatorData);
  for (const auto & ag : mAgents)
    part->mAgents.emplace_back(ag->Clone());
  return part;
}
