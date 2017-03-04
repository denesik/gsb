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
  if(!res)
  {
    mSectors.emplace(std::make_pair(pos, res));
    res.lock()->CreateRenderData();
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

void World::SetLoader(std::unique_ptr<IMapLoader> loader)
{
  mLoader = std::move(loader);
}

