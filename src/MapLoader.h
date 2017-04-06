#pragma once
#include "IMapLoader.h"
#include "IMapGenerator.h"
#include <memory>
#include "ThreadProcess.h"

class MapLoader : public ThreadProcess_old<MapLoader>
{
public:
  MapLoader(const IMapGenerator &generator);
  ~MapLoader();

  void SetSector(std::weak_ptr<Sector> sector);

  void Process();

private:
  std::weak_ptr<Sector> mSector;
  const IMapGenerator &mGenerator;

};

