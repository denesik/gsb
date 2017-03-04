#include "MapLoader.h"
#include "Sector.h"

MapLoader::MapLoader(World &world, std::unique_ptr<IMapGenerator> generator) : mGenerator(std::move(generator)), mWorld(world)
{
  mUpdate = std::thread([this]()
  {
    while (!mUpdateEnd)
    {
      Update();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  });
}

MapLoader::~MapLoader()
{
  mUpdateEnd = true;
  mUpdate.join();
}

std::weak_ptr<Sector> MapLoader::GetSector(SPos pos)
{
  {
    mSectorLock.lock();
    auto find = mSectors.find(pos);
    if (find != mSectors.end())
    {
      mSectorLock.unlock();
      return find->second;
    }

    mSectorLock.unlock();
  }

  {
    mQueueLock.lock();
    auto find = mQueue.find(pos);
    if (find == mQueue.end())
      mQueue.emplace(pos);
    mQueueLock.unlock();
  }

  return{};
}

void MapLoader::Update()
{
  mQueueLock.lock();
  if(mQueue.empty())
  {
    mQueueLock.unlock();
    return;
  }
  auto pos = *mQueue.begin();
  mQueue.erase(mQueue.begin());
  mQueueLock.unlock();

  auto sector = std::make_shared<Sector>(mWorld, pos);
  sector->ApplyGenerator(*mGenerator);

  mSectorLock.lock();
  mSectors.emplace(std::make_pair(pos, sector));
  mSectorLock.unlock();
}
