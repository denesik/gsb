#include "World.h"
#include "MapLoader.h"
#include "WorldGeneratorTest.h"
#include "WorldGeneratorFlat.h"
#include "ChunkHelper.h"
#include "Sector.h"

// 1. Создаем 9 секторов.
// 2. Загружаем 9 секторов.
// 3. Если все 9 секторов загружены - кешируем сектора и вызываем событие загрузки.

World::World(const DataBase &blocksDataBase)
  : mBlocksDataBase(blocksDataBase), 
  mFakeSector(*this, SPos{}),
  mUpdatableSectors(*this), 
  mPlayer(*this), 
  mWorldGenerator(std::make_unique<WorldGeneratorBiome>()),
  mWorldLoader(std::make_unique<MapLoaderFromGenerator>(*mWorldGenerator, blocksDataBase)),
  mSectorLoader(1, 1, *mWorldLoader)
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
        UseSector(pos + offset);
        mSectorLoader.Push(it->second.sector);
      }
    }
  }
}

// Освобождаем себя и всех соседей.
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
    // Создали сектор, сообщим ему и всем его соседям об этом.
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
        // Если сектор нужно выгрузить, кешируем и посылаем события ему и его соседям, если они были загружены.
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

  // Пробегаем по всем секторам в области и формируем события если надо.
  for (const auto &offset : gSectorNeighboard)
  {
    auto it = mSectors.find(pos + offset);
    if (it != mSectors.end())
    {
      // Сектор и все его соседи загружены.
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

boost::optional<Sector &> World::GetSector(const SPos &pos) const
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    return const_cast<Sector&>(it->second.sector);
  }
  return{};
}

BlockId World::GetBlockId(const WBPos& pos) const
{
   auto spos = cs::WBtoS(pos);
   auto sector = GetSector(spos);
   if(sector.is_initialized())
   {
     return sector->GetBlockId(cs::WBtoSB(pos));
   }

  return{};
}

Block* World::GetBlockDynamic(const WBPos& pos) const
{
  auto spos = cs::WBtoS(pos);
  auto sector = GetSector(spos);
  if (sector.is_initialized())
  {
    return sector->GetBlockDynamic(cs::WBtoSB(pos));
  }

  return {};
}

bool World::CreateBlock(const WBPos& pos, BlockId id)
{
  auto spos = cs::WBtoS(pos);
  auto sector = GetSector(spos);
  if (sector.is_initialized())
  {
    sector->CreateBlock(cs::WBtoSB(pos), id);
    return true;
  }

  return false;
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

  // Сначала выгружаем сектора.
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

  // Затем загружаем.
  mSectorLoader.Update();
}


