#include "World.h"
#include "MapLoader.h"
#include "WorldGeneratorFlat.h"
#include "ChunkHelper.h"
#include "Sector.h"

World::World(const DataBase &blocksDataBase)
  : mBlocksDataBase(blocksDataBase)
  , mUpdatableSectors(*this)
  , mPlayer(*this)
  , mWorldGenerator(std::make_unique<WorldGeneratorBiome>())
  , mLoaderWorker(std::make_unique<ThreadWorker<TaskGenerate, MapLoaderFromGenerator>>(*mWorldGenerator, blocksDataBase))
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
      mLoaderWorker->Add({ *this, pos });
    }
  }
}

void World::UnLoadSector(const SPos &pos)
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    UnloadSectorEvent(*it->second);
    mSectors.erase(it);
  }
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

void World::DestroyBlock(const WBPos & pos)
{
  auto spos = cs::WBtoS(pos);
  auto sector = GetSector(spos);
  if (!sector.expired())
  {
    auto shared = sector.lock();
    return shared->CreateBlock(cs::WBtoSB(pos), 1);
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
  mLoaderWorker->Update();
  mUpdatableSectors.Update();
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

TaskGenerate::TaskGenerate(World &morld, const SPos &pos)
{
  mSector = std::make_shared<Sector>(morld, pos);
}

bool TaskGenerate::Begin(IMapLoader &loader)
{
  loader.SetSector(mSector);
  return true;
}

void TaskGenerate::End(const IMapLoader &loader)
{
  auto &world = mSector->GetWorld();
  auto &res = world.mSectors.emplace(std::make_pair(mSector->GetPos(), mSector));
  if (res.second)
  {
    // TODO: узнать почему может вернуться false
    world.LoadSectorEvent(*(res.first->second));
  }
  world.mLoadedSectors.erase(mSector->GetPos());
}
