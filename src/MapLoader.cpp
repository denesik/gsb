#include "MapLoader.h"
#include "Sector.h"


MapLoader::MapLoader(const IMapGenerator &generator)
  : mGenerator(generator)
{
  mThread = std::thread([this]()
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

MapLoader::~MapLoader()
{
  mClose = true;
  mCv.notify_one();
  mThread.join();
}

void MapLoader::SetSector(std::weak_ptr<Sector> sector)
{
  mSector = sector;
}

void MapLoader::Run()
{
  mRunned = true;
  mCv.notify_one();
}

bool MapLoader::IsDone() const
{
  return mRunned == false;
}

void MapLoader::Process()
{
  if (!mSector.expired())
  {
    mGenerator.Generate(*mSector.lock());
  }
}
