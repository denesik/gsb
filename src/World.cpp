#include "World.h"
#include "MapLoader.h"
#include "BiomeMapGenerator.h"
#include "WorldGeneratorTest.h"
#include "WorldGeneratorFlat.h"
#include "ChunkHelper.h"
#include "Sector.h"

// 1. —оздаем 9 секторов.
// 2. «агружаем 9 секторов.
// 3. ≈сли все 9 секторов загружены - кешируем сектора и вызываем событие загрузки.

World::World(const DataBase &blocksDataBase)
  : mBlocksDataBase(blocksDataBase), 
  mFakeSector(*this, SPos{}),
  mUpdatableSectors(*this), 
  mPlayer(*this), 
  mWorldGenerator(std::make_unique<WorldGeneratorFlat>(blocksDataBase)),
  mSectorLoader(1, 1, *mWorldGenerator)
{
  mSectorLoader.SetBeginCallback([this](Worker &worker, Sector &sector)
  {
    return OnSectorLoadBegin(worker, sector);
  });

  mSectorLoader.SetEndCallback([this](Worker &worker, Sector &sector)
  {
    return OnSectorLoadEnd(worker, sector);
  });
}


World::~World()
{
}


// —оздаем себ€ и всех соседей.
// ≈сли кто то не был поставлен на загрузку, ставим на загрузку.
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
        UseSector(pos + offset);
        mSectorLoader.Push(it->second.sector);
      }
    }
  }
}

// ќсвобождаем себ€ и всех соседей.
void World::UnLoadSector(const SPos &pos)
{
  mUnloadSectors.emplace_back(pos);
}

void World::UseSector(const SPos &pos)
{
  auto it = mSectors.find(pos);
  if (it == mSectors.end())
  {
    auto &res = mSectors.emplace(std::piecewise_construct,
      std::forward_as_tuple(pos),
      std::forward_as_tuple(*this, pos));
    
    if (!res.second)
      return;
    it = res.first;
    // —оздали сектор, сообщим ему и всем его сосед€м об этом.
  }
  ++it->second.count;
}

void World::UnuseSector(const SPos &pos)
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    if (it->second.count == 1 && it->second.loaded)
    {
        // ≈сли сектор нужно выгрузить, кешируем и посылаем событи€ ему и его сосед€м, если они были загружены.
        for (const auto &offset : gSectorNeighboard)
        {
          auto jt = mSectors.find(pos + offset);
          if (jt != mSectors.end())
          {
            if (CountLoaded(pos + offset) == gSectorNeighboard.size())
            {
              mWorldSectorObserver.UnLoad(jt->second.sector);
              CacheSector(pos + offset, Sector::CACHE_UNLOAD);
            }
          }
        }
    }

    --it->second.count;
    if (!it->second.count)
    {
      mSectors.erase(it);
    }
  }
}

size_t World::CountLoaded(const SPos &pos)
{
  size_t count = 0;
  for (const auto &offset : gSectorNeighboard)
  {
    auto it = mSectors.find(pos + offset);
    if (it != mSectors.end() && it->second.count)
    {
      count += it->second.loaded;
    }
  }

  return count;
}

void World::CacheSector(const SPos &pos, Sector::CacheState state)
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    // —ообщаем текущему сектору что он загружен.
    // —ообщаем всем сосед€м текущего сектора что сектор загружен.
    // —ообщаем текущему сектору о всех его сосед€х.
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

bool World::OnSectorLoadBegin(Worker &worker, Sector &sector)
{
  return true;
}

bool World::OnSectorLoadEnd(Worker &worker, Sector &sector)
{
  const auto pos = sector.GetPos();

  UnuseSector(pos);

  {
    auto it = mSectors.find(pos);
    if (it != mSectors.end())
    {
      it->second.loaded = true;
    }
  }

  // ѕробегаем по всем секторам в области и формируем событи€ если надо.
  for (const auto &offset : gSectorNeighboard)
  {
    auto it = mSectors.find(pos + offset);
    if (it != mSectors.end())
    {
      // —ектор и все его соседи загружены.
      if (CountLoaded(pos + offset) == gSectorNeighboard.size())
      {
        CacheSector(pos + offset, Sector::CACHE_LOAD);
        mWorldSectorObserver.Load(it->second.sector);
      }
    }
  }
  return true;
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


  mSectorLoader.Update();

  if (!mUnloadSectors.empty())
  {
    for (const auto &pos : mUnloadSectors)
    {
      for (const auto &offset : gSectorNeighboard)
      {
        UnuseSector(pos + offset);
      }
    }

    mUnloadSectors.clear();
  }
}


