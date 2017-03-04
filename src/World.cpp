#include "World.h"
#include "MapGenerator.h"


World::World(const BlocksDataBase &blocksDataBase, std::unique_ptr<IMapGenerator> gen)
  : mBlocksDataBase(blocksDataBase), mUpdatableSectors(*this), mGenerator(std::move(gen))
{
  LoadSector({});
}


World::~World()
{
}

void World::LoadSector(const SPos &pos)
{
  auto t = std::make_shared<Sector>(*this, pos);
  t->ApplyGenerator(*mGenerator);
  mSectors.insert({ pos , t });
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

