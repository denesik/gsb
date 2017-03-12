#include "MapLoader.h"
#include "Sector.h"

/*  mThread = std::thread([this]()
  {
    while (!mClose)
    {
      std::unique_lock<std::mutex> lock(mMutex);
      if (mRunned)
      {
        Process();
        mRunned = false;
      }
      mCv.wait(lock);
    }
  });
}

SectorCompiler::~SectorCompiler()
{
  mClose = true;
  mCv.notify_one();
  mThread.join();
}*/

MapLoader::MapLoader(std::unique_ptr<IMapGenerator> generator) : IMapLoader(std::move(generator))
{
  mThread = std::thread([this]()
  {
    while (!mThreadStop)
    {
      std::unique_lock<std::mutex> lock(mMutex);
      if (mRunned)
      {
        Process();
        mRunned = false;
      }
      mCv.wait(lock);
    }
  });
}

MapLoader::~MapLoader()
{
  mThreadStop = true;
  mCv.notify_one();
  mThread.join();
}

std::shared_ptr<Sector> MapLoader::GetSector(SPos pos)
{
  {
    mSectorLock.lock();
    auto find = mSectors.find(pos);
    if (find != mSectors.end())
    {
      auto sector = find->second;
      mSectors.erase(find);
      mSectorLock.unlock();
      return sector;
    }

    mSectorLock.unlock();
  }

  {
    mQueueLock.lock();
    auto find = mQueue.find(pos);
    if (find == mQueue.end())
      mQueue.emplace(pos);
    mQueueLock.unlock();

    Run();
  }

  return{};
}

void MapLoader::Run()
{
    mRunned = true;
    mCv.notify_one();
}

void MapLoader::Process()
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

  auto sector = std::make_shared<Sector>(*mWorld, pos);
  sector->ApplyGenerator(*mGenerator);

  mSectorLock.lock();
  mSectors.emplace(std::make_pair(pos, sector));
  mSectorLock.unlock();
}
