#pragma once
#include "IMapLoader.h"
#include "IMapGenerator.h"
#include <memory>
#include "ThreadProcess.h"
#include <DataBase.h>

class MapLoader : public IMapLoader
{
public:
  MapLoader(const IMapGenerator &generator, const DataBase &db);

  void Process();
};

