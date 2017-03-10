#include "BlockDynamicPart.h"
#include <Magnum/Timeline.h>
#include "BlocksDataBase.h"
#include <algorithm>

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
    part->mAgents.push_back(ag->Clone(this));
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

size_t BlockDynamicPart::AddAgent(std::unique_ptr<Agent> agent)
{
  //auto res = mAgents.insert(std::make_pair(agent->Id(), std::move(agent)));
  //return res.second;
  mAgents.push_back(std::move(agent));
  return mAgents.size() - 1;
}

bool BlockDynamicPart::Binding(size_t index, SideIndex side, AgentDirection dir)
{
  auto &vec = dir == AgentDirection::in ? mBindingSide[side].in : mBindingSide[side].out;

  vec.push_back(index);

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
  auto &vec = dir == AgentDirection::in ? mBindingSide[side].in : mBindingSide[side].out;

  auto it = std::find_if(vec.begin(), vec.end(), [this, type](size_t val)
  {
    return mAgents[val]->Id() == type;
  });

  if (it != vec.end())
  {
    return{ *mAgents[*it] };
  }

  return{};
}

BlockDynamicPart *BlockDynamicPart::GetNeighbour(SideIndex side)
{
  return m_sector.GetBlockDynamic(cs::Side(cs::BItoSB(pos), side));
}
