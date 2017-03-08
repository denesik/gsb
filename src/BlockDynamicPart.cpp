#include "BlockDynamicPart.h"
#include <Magnum/Timeline.h>


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
    part->mAgents.insert(std::make_pair(ag.second->Id(), ag.second->Clone()));
  return part;
}

void BlockDynamicPart::DrawGui(Magnum::Timeline dt) const
{
  for (const auto & ag : mAgents)
    ag.second->DrawGui(dt);
}
