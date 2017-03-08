#include "BlockDynamicPart.h"
#include <Magnum/Timeline.h>
#include "BlocksDataBase.h"


BlockDynamicPart::BlockDynamicPart(BlockId id, const BlocksDataBase &db)
  : mBlockId(id), mDb(db)
{
}


BlockDynamicPart::BlockDynamicPart(const BlockDynamicPart &other)
  : mBlockId(other.mBlockId), mDb(other.mDb)
{
  if (other.mTesselatorData) mTesselatorData = std::make_unique<TesselatorData>(*mTesselatorData);
}

BlockDynamicPart::~BlockDynamicPart()
{
}

std::unique_ptr<BlockDynamicPart> BlockDynamicPart::Clone()
{
  auto part = std::make_unique<BlockDynamicPart>(*this);
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

const std::unique_ptr<BlockStaticPart> & BlockDynamicPart::GetStaticPart() const
{
  return mDb.GetBlockStaticPart(mBlockId);
}

const BlocksDataBase &BlockDynamicPart::GetDataBase() const
{
  return mDb;
}
