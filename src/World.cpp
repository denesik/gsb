#include "World.h"
#include "MapLoader.h"
#include "BiomeMapGenerator.h"
#include "WorldGeneratorTest.h"
#include "WorldGeneratorFlat.h"

World::World(const DataBase &blocksDataBase)
  : mBlocksDataBase(blocksDataBase), mUpdatableSectors(*this), mPlayer(*this), mWorldGenerator(std::make_unique<WorldGeneratorFlat>(blocksDataBase)), mLoaderWorker(*mWorldGenerator)
{
}


World::~World()
{
}

void World::LoadSector(const SPos &pos)
{
  // Если сектора нет на карте и сектор не загружается, поставим его на загрузку.
  auto sector = GetSector(pos);
  if (sector.expired())
  {
    auto it = mLoadedSectors.find(pos);
    if (it == mLoadedSectors.end())
    {
      mLoadedSectors.emplace(pos);
      mLoaderWorker.Add({ *this, pos });
    }
  }
}

void World::UnLoadSector(const SPos &pos)
{
  mSectors.erase(pos);
}

const DataBase & World::GetBlocksDataBase() const
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

Block* World::GetBlockDynamic(const WBPos& pos) const
{
  auto spos = cs::WBtoS(pos);
  auto sector = GetSector(spos);
  if (!sector.expired())
  {
    auto shared = sector.lock();
    return shared->GetBlockDynamic(cs::WBtoSB(pos));
  }

  return {};
}

void World::CreateBlock(const WBPos& pos, BlockId id)
{
  auto spos = cs::WBtoS(pos);
  auto sector = GetSector(spos);
  if (!sector.expired())
  {
    auto shared = sector.lock();
    return shared->CreateBlock(cs::WBtoSB(pos), id);
  }
}

UpdatableSectors & World::GetUpdatableSectors()
{
  return mUpdatableSectors;
}

IMapGenerator & World::GetWorldGenerator()
{
  return *mWorldGenerator;
}

void World::Wipe()
{
	mSectors.clear();
}

void World::Update()
{
  mLoaderWorker.Update();
  mUpdatableSectors.Update();
}

TaskGenerate::TaskGenerate(World &morld, const SPos &pos)
{
  mSector = std::make_shared<Sector>(morld, pos);
}

bool TaskGenerate::Begin(MapLoader &loader)
{
  loader.SetSector(mSector);
  return true;
}

void TaskGenerate::End(const MapLoader &loader)
{
  auto &world = mSector->GetWorld();
  world.mSectors.emplace(std::make_pair(mSector->GetPos(), mSector));
  world.mLoadedSectors.erase(mSector->GetPos());
}
