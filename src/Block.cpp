#include "Block.h"
#include <Magnum/Timeline.h>
#include "DataBase.h"
#include <algorithm>
#include "Sector.h"

Block::Block(const Block &other)
  : Block(other, other.m_sector)
{
}


Block::Block(Block &&other)
  : Block(std::move(other), other.m_sector)
{

}

Block::Block(const Block &other, Sector &parent)
  : mBlockId(other.mBlockId), mDb(other.mDb), m_sector(parent)
{
  for (const auto & ag : other.mAgents)
    mAgents.push_back(ag->Clone(*this));
}

Block::Block(Block &&other, Sector &parent)
  : mBlockId(other.mBlockId), mDb(other.mDb), m_sector(parent), mAgents(std::move(other.mAgents))
{

}

// TODO: проверить исключения.
Block::Block(const DataBase &db, const rapidjson::Value &val, Sector &parent, BlockId id)
  : mBlockId(id), mDb(db), m_sector(parent)
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
          mAgents.push_back(std::move(accessor));
        }
      }
    }
  }
}

const std::unique_ptr<StaticBlock> & Block::GetStaticPart() const
{
  return mDb.GetBlockStaticPart(mBlockId);
}

const DataBase &Block::GetDataBase() const
{
  return mDb;
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
  return m_sector.GetBlockDynamic(cs::Side(cs::BItoSB(mPos), side));
}

void Block::SetPos(IndexType pos)
{
  mPos = pos;
}

boost::optional<Accessor &> Block::GetAccessorByName(AccessorId name) const
{
  auto it = std::find_if(mAgents.begin(), mAgents.end(), [name](const decltype(mAgents)::value_type &val)
  {
    return val->Name() == name;
  });

  if (it != mAgents.end())
  {
    return{ **it };
  }

  return{};
}

boost::optional<Accessor &> Block::GetAccessorByName(const std::string &name) const
{
  return GetAccessorByName(Accessor::NameFromString(name));
}

