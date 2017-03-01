#include "World.h"



World::World(const BlocksDataBase &blocksDataBase)
  : mBlocksDataBase(blocksDataBase)
{
  LoadSector({});
}


World::~World()
{
}

void World::LoadSector(const SPos &pos)
{
  mSectors.insert({ pos , std::make_shared<Sector>(this) });
}

void World::UnLoadSector(const SPos &pos)
{
  mSectors[pos] = nullptr;
}

const BlocksDataBase & World::GetBlocksDataBase() const
{
  return mBlocksDataBase;
}

std::shared_ptr<Sector> World::GetSector(const SPos &pos)
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    return it->second;
  }
  return nullptr;
}
