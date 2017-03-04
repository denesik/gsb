#include "World.h"
#include "MapGenerator.h"


World::World(const BlocksDataBase &blocksDataBase, std::unique_ptr<IMapLoader> loader)
  : mBlocksDataBase(blocksDataBase), mUpdatableSectors(*this), mLoader(std::move(loader))
{
  LoadSector({});
}


World::~World()
{
}

void World::LoadSector(const SPos &pos)
{
  auto res = mLoader->GetSector(pos);
  if(!res.expired() && res.lock())
  {
    res.lock()->CreateRenderData();
    mSectors.emplace(std::make_pair(pos, res.lock()));
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

std::weak_ptr<Sector> World::GetSector(const SPos &pos)
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    return {it->second};
  }
  return {};
}

UpdatableSectors & World::GetUpdatableSectors()
{
  return mUpdatableSectors;
}

