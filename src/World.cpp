#include "World.h"
#include "MapGenerator.h"


World::World(const BlocksDataBase &blocksDataBase)
  : mBlocksDataBase(blocksDataBase), mUpdatableSectors(*this), mPlayer(*this)
{
}


World::~World()
{
}

void World::LoadSector(const SPos &pos)
{
  assert(mLoader != nullptr);
  auto res = mLoader->GetSector(pos);
  //auto res = std::make_shared<Sector>(*this, pos);
  if(res)
  {
    mSectors.emplace(std::make_pair(pos, res));
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

std::weak_ptr<Sector> World::GetSector(const SPos &pos) const
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    return {it->second};
  }
  return {};
}

std::vector<std::weak_ptr<Sector>> World::GetColumn(const CSPos& pos) const
{
  std::vector<std::weak_ptr<Sector>> column;
  column.reserve(SECTOR_COUNT_HEIGHT);

  for (Magnum::Int c = 0; c < SECTOR_COUNT_HEIGHT; ++c)
  {
    column.emplace_back(GetSector({ pos, c }));
  }

  return  column;
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

void World::SetLoader(std::unique_ptr<IMapLoader> loader)
{
  mLoader = std::move(loader);
}

