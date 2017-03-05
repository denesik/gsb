#pragma once
#include "IMapLoader.h"
#include "IMapGenerator.h"
#include <unordered_map>
#include <mutex>
#include <set>
#include <atomic>
#include <unordered_set>

class World;

class MapLoader : public IMapLoader
{
public:
  MapLoader(std::unique_ptr<IMapGenerator> generator);
  ~MapLoader();

  std::shared_ptr<Sector> GetSector(SPos pos) override;

  void Run();

private:
  void Process();

  std::unique_ptr<IMapGenerator> mGenerator;

  std::unordered_map<SPos, std::shared_ptr<Sector>> mSectors;
  std::mutex mSectorLock;

  std::unordered_set<SPos> mQueue;
  std::mutex mQueueLock;

  std::thread mThread;
  std::atomic<bool> mThreadStop = false;  

  std::mutex mMutex;
  std::condition_variable mCv;
  std::atomic<bool> mRunned = false;
};
