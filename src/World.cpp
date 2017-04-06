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

void World::LoadSector(const SPos &pos)
{
  for (const auto &offset : gSectorNeighboard)
  {
    if (UseSector(pos + offset) == 1)
    {

    };
  }
}

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
    // ��������� ������, ������� ��� � ���� ��� ������� �� ����.

    it = res.first;
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
      // ��������� ������, ������� ��� � ���� ��� ������� �� ����.

      mSectors.erase(it);
    }
  }
  return count;
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
	mSectors.clear();
}

void World::Update()
{
  //mUpdatableSectors.Update();
}

void World::LoadSectorEvent(Sector &sector)
{
  // �������� �������� ������� ��� �� ��������.
  // �������� ���� ������� �������� ������� ��� ������ ��������.
  // �������� �������� ������� � ���� ��� �������.
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
  // �������� �������� ������� ��� �� ��������.
  // �������� ���� ������� �������� ������� ��� ������ ��������.
  // �������� �������� ������� � ���� ��� �������.
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


