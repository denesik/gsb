#include "World.h"



World::World(const BlocksDataBase &blocksDataBase)
  : mBlocksDataBase(blocksDataBase), mUpdatableSectors(*this)
{
  LoadSector({});
}


World::~World()
{
}

void World::LoadSector(const SPos &pos)
{
  mSectors.insert({ pos , std::make_shared<Sector>(*this, pos) });
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

