#include "World.h"
#include "MapLoader.h"
#include "BiomeMapGenerator.h"
#include "WorldGeneratorTest.h"
#include "WorldGeneratorFlat.h"
#include "ChunkHelper.h"
#include "Sector.h"

World::World(const DataBase &blocksDataBase)
  : mBlocksDataBase(blocksDataBase), 
  mFakeSector(*this, SPos{}),
  mUpdatableSectors(*this), 
  mPlayer(*this), 
  mWorldGenerator(std::make_unique<BiomeMapGenerator>(blocksDataBase)), 
  mLoaderWorker(*mWorldGenerator)
{
}


World::~World()
{
}


// 108 calls find
// Создаем себя и всех соседей.
// Если кто то не был поставлен на загрузку, ставим на загрузку.
void World::LoadSector(const SPos &pos)
{
  for (const auto &offset : gSectorNeighboard)
  {
    UseSector(pos + offset);
    auto it = mSectors.find(pos + offset);
    if (it != mSectors.end())
    {
      if (!it->second.added)
      {
        it->second.added = true;
        // Генерируем сектор.
      }
    }
  }
}

// Освобождаем себя и всех соседей.
void World::UnLoadSector(const SPos &pos)
{
  for (const auto &offset : gSectorNeighboard)
  {
    UnuseSector(pos + offset);
  }
}

size_t World::UseSector(const SPos &pos)
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    auto &res = mSectors.emplace(pos, this, pos);
    if (!res.second)
      return 0;
    it = res.first;
    // Создали сектор, сообщим ему и всем его соседям об этом.
    CacheSector(pos, Sector::CACHE_LOAD);
  }
  ++it->second.count;
  return it->second.count;
}

size_t World::UnuseSector(const SPos &pos)
{
  auto count = 0;
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    --it->second.count;
    count = it->second.count;
    if (!it->second.count)
    {
      // Выгрузили сектор, сообщим ему и всем его соседям об этом.
      CacheSector(pos, Sector::CACHE_UNLOAD);
      mSectors.erase(it);
    }
  }
  return count;
}

void World::CacheSector(const SPos &pos, Sector::CacheState state)
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    // Сообщаем текущему сектору что он загружен.
    // Сообщаем всем соседям текущего сектора что сектор загружен.
    // Сообщаем текущему сектору о всех его соседях.
    it->second.sector.Cache(it->second.sector, state);
    for (size_t i = 0; i < SectorAround::pos.size(); ++i)
    {
      auto sec_it = mSectors.find(pos + SectorAround::pos[i]);
      if (sec_it != mSectors.end())
      {
        if (it->second.sector.GetPos() != sec_it->second.sector.GetPos())
        {
          sec_it->second.sector.Cache(it->second.sector, state);
          it->second.sector.Cache(sec_it->second.sector, state);
        }
      }
    }
  }
}

const DataBase & World::GetBlocksDataBase() const
{
  return mBlocksDataBase;
}

// boost::optional<Sector> World::GetSector(const SPos &pos) const
// {
//   auto it = mSectors.find(pos);
//   if (it != mSectors.end())
//   {
//     return{ it->second };
//   }
//   return{};
// }

BlockId World::GetBlockId(const WBPos& pos) const
{
//   auto spos = cs::WBtoS(pos);
//   auto sector = GetSector(spos);
//   if(!sector.expired())
//   {
//     auto shared = sector.lock();
//     return shared->GetBlockId(cs::WBtoSB(pos));
//   }

  return{};
}

Block* World::GetBlockDynamic(const WBPos& pos) const
{
//   auto spos = cs::WBtoS(pos);
//   auto sector = GetSector(spos);
//   if (!sector.expired())
//   {
//     auto shared = sector.lock();
//     return shared->GetBlockDynamic(cs::WBtoSB(pos));
//   }

  return {};
}

void World::CreateBlock(const WBPos& pos, BlockId id)
{
//   auto spos = cs::WBtoS(pos);
//   auto sector = GetSector(spos);
//   if (!sector.expired())
//   {
//     auto shared = sector.lock();
//     return shared->CreateBlock(cs::WBtoSB(pos), id);
//   }
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

}

void World::Update()
{
  //mUpdatableSectors.Update();
}

void World::LoadSectorEvent(Sector &sector)
{
  // Сообщаем текущему сектору что он загружен.
  // Сообщаем всем соседям текущего сектора что сектор загружен.
  // Сообщаем текущему сектору о всех его соседях.
  sector.LoadSector(sector);
  for (size_t i = 0; i < SectorAround::pos.size(); ++i)
  {
    auto s = GetSector(sector.GetPos() + SectorAround::pos[i]);
    if (!s.expired())
    {
      auto shared = s.lock();
      if (sector.GetPos() != shared->GetPos())
      {
        shared->LoadSector(sector);
        sector.LoadSector(*shared);
      }
    }
  }

  mWorldSectorObserver.Load(sector);
}

void World::UnloadSectorEvent(Sector &sector)
{
  // Сообщаем текущему сектору что он выгружен.
  // Сообщаем всем соседям текущего сектора что сектор выгружен.
  // Сообщаем текущему сектору о всех его соседях.
  sector.UnloadSector(sector);
  for (size_t i = 0; i < SectorAround::pos.size(); ++i)
  {
    auto s = GetSector(sector.GetPos() + SectorAround::pos[i]);
    if (!s.expired())
    {
      auto shared = s.lock();
      if (sector.GetPos() != shared->GetPos())
      {
        shared->UnloadSector(sector);
        sector.UnloadSector(*shared);
      }
    }
  }

  mWorldSectorObserver.UnLoad(sector);
}


