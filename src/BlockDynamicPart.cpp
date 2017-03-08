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
  if (GetTesselatorData()) part->GetTesselatorData() = std::make_unique<TesselatorData>(*GetTesselatorData());
  for (const auto & ag : mAgents)
    part->mAgents.insert(std::make_pair(ag.second->Id(), ag.second->Clone(this)));
  return part;
}

void BlockDynamicPart::DrawGui(const Magnum::Timeline &dt)
{
  for (const auto & ag : mAgents)
    ag.second->DrawGui(dt);
}

std::unique_ptr<TesselatorData> & BlockDynamicPart::GetTesselatorData()
{
  return mTesselatorData;
}

const std::unique_ptr<TesselatorData> & BlockDynamicPart::GetTesselatorData() const
{
  return mTesselatorData;
}

bool BlockDynamicPart::AddAgent(std::unique_ptr<Agent> agent)
{
  auto res = mAgents.insert(std::make_pair(agent->Id(), std::move(agent)));
  return res.second;
}
