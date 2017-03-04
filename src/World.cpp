#include "World.h"
#include "MapGenerator.h"


World::World(const BlocksDataBase &blocksDataBase)
  : mBlocksDataBase(blocksDataBase), mUpdatableSectors(*this)
{
}


World::~World()
{
}

void World::LoadSector(const SPos &pos)
{
  assert(mLoader != nullptr);
  auto res = mLoader->GetSector(pos);
  //auto res = std::make_shared<Sector>(*this, pos);
  if(res)
  {
    mSectors.emplace(std::make_pair(pos, res));
  }
}

void World::UnLoadSector(const SPos &pos)
{
  mSectors.erase(pos);
}

const BlocksDataBase & World::GetBlocksDataBase() const
{
  return mBlocksDataBase;
}

std::weak_ptr<Sector> World::GetSector(const SPos &pos) const
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    return {it->second};
  }
  return {};
}

BlockId World::GetBlockId(const WBPos& pos) const
{
  auto spos = cs::WBtoS(pos);
  auto sector = GetSector(spos);
  if(!sector.expired())
  {
    auto shared = sector.lock();
    return shared->GetBlockId(cs::WBtoSB(pos));
  }

  return{};
}

void World::SetBlockId(const WBPos& pos, BlockId id)
{
  auto spos = cs::WBtoS(pos);
  auto sector = GetSector(spos);
  if (!sector.expired())
  {
    auto shared = sector.lock();
    return shared->SetBlockId(cs::WBtoSB(pos), id);
  }
}

UpdatableSectors & World::GetUpdatableSectors()
{
  return mUpdatableSectors;
}

void World::SetLoader(std::unique_ptr<IMapLoader> loader)
{
  mLoader = std::move(loader);
}

