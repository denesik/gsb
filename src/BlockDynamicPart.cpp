#include "BlockDynamicPart.h"
#include <Magnum/Timeline.h>
#include "BlocksDataBase.h"
#include <algorithm>
#include "Sector.h"

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
    //part->mAgents.insert(std::make_pair(ag.second->Id(), ag.second->Clone(this)));
    part->mAgents.push_back(ag->Clone(part.get()));
  return part;
}

void BlockDynamicPart::DrawGui(const Magnum::Timeline &dt)
{
  for (const auto & ag : mAgents)
    ag->DrawGui(dt);
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
  mAgents.push_back(std::move(agent));
  return true;
}

const std::unique_ptr<BlockStaticPart> & BlockDynamicPart::GetStaticPart() const
{
  return mDb.GetBlockStaticPart(mBlockId);
}

const BlocksDataBase &BlockDynamicPart::GetDataBase() const
{
  return mDb;
}

boost::optional<Agent &> BlockDynamicPart::GetAgent(AgentId type, SideIndex side, AgentDirection dir)
{
  auto it = std::find_if(mAgents.begin(), mAgents.end(), [this, type, side, dir](const decltype(mAgents)::value_type &val)
  {
    return val->Id() == type && ((val->GetDirection(side) & dir) == dir);
  });

  if (it != mAgents.end())
  {
    return{ **it };
  }

  return{};
}

BlockDynamicPart *BlockDynamicPart::GetNeighbour(SideIndex side)
{
  return m_sector->GetBlockDynamic(cs::Side(cs::BItoSB(pos), side));
}
