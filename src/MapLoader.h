#pragma once
#include "IMapLoader.h"
#include "IMapGenerator.h"
#include <unordered_map>
#include <mutex>
#include <set>
#include <atomic>
#include <unordered_set>
#include <memory>

class MapLoader
{
public:
  MapLoader(const IMapGenerator &generator);
  ~MapLoader();

  void SetSector(std::weak_ptr<Sector> sector);

  void Run();

  bool IsDone() const;

private:
  std::thread mThread;
  std::atomic<bool> mRunned = false;
  std::atomic<bool> mClose = false;
  std::condition_variable mCv;
  std::mutex mMutex;

  std::weak_ptr<Sector> mSector;
  const IMapGenerator &mGenerator;

private:
  void Process();
};

