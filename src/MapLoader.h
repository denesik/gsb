#pragma once
#include "IMapLoader.h"
#include "IMapGenerator.h"
#include <memory>
#include "ThreadProcess.h"
#include <DataBase.h>

class MapLoader : public ThreadProcess<MapLoader>
{
public:
  MapLoader(const IMapGenerator &generator, const DataBase &db);
  ~MapLoader();

  void SetSector(std::weak_ptr<Sector> sector);

  void Process();

private:
  std::weak_ptr<Sector> mSector;
  const IMapGenerator &mGenerator;
  const DataBase &mDb;
};

