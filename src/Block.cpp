#include "Block.h"
#include <Magnum/Timeline.h>
#include "DataBase.h"
#include <algorithm>
#include "Sector.h"

Block::Block()
{
}


Block::Block(const Block &other)
  : mBlockId(other.mBlockId), mDb(other.mDb)
{
  if (other.mTesselatorData) mTesselatorData = std::make_unique<TesselatorData>(*mTesselatorData);
  for (const auto & ag : other.mAgents)
    mAgents.push_back(ag->Clone(*this));
}

// TODO: проверить исключения.
Block::Block(const DataBase & db, const rapidjson::Value &val)
{
  if (val.HasMember("agents"))
  {
    const rapidjson::Value &arr = val["agents"];
    if (val["agents"].IsArray())
    {
      for (decltype(arr.Size()) a = 0; a < arr.Size(); a++)
      {
        const auto& part = arr[a];
        if (part.HasMember("type"))
        {
          std::string agenttype = part["type"].GetString();
          auto accessor = Accessor::factory::create(agenttype, db, part, *this);
          if (!accessor)
          {
            continue;
          }
          AddAgent(std::move(accessor));
        }
      }
    }
  }
}

Block::~Block()
{
}

std::unique_ptr<Block> Block::Clone()
{
  return std::make_unique<Block>(*this);
}

void Block::DrawGui(const Magnum::Timeline &dt)
{
  for (const auto & ag : mAgents)
  {
    ag->DrawGui(dt);
  }
    
}

std::unique_ptr<TesselatorData> & Block::GetTesselatorData()
{
  return mTesselatorData;
}

const std::unique_ptr<TesselatorData> & Block::GetTesselatorData() const
{
  return mTesselatorData;
}

bool Block::AddAgent(std::unique_ptr<Accessor> accessor)
{
  mAgents.push_back(std::move(accessor));
  return true;
}

const std::unique_ptr<StaticBlock> & Block::GetStaticPart() const
{
  return mDb->GetBlockStaticPart(mBlockId);
}

const DataBase &Block::GetDataBase() const
{
  return *mDb;
}

boost::optional<Accessor &> Block::GetAgent(AccessorId type, SideIndex side, AccessorDirection dir)
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

Block *Block::GetNeighbour(SideIndex side)
{
  return m_sector->GetBlockDynamic(cs::Side(cs::BItoSB(pos), side));
}

