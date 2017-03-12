#pragma once
#include "tools/CoordSystem.h"
#include <memory>
#include <tools/Common.h>
#include "IMapGenerator.h"

class Sector;
class World;

class GSB_NOVTABLE IMapLoader
{
public:
  IMapLoader(std::unique_ptr<IMapGenerator>);
  virtual ~IMapLoader() = default;
  virtual std::shared_ptr<Sector> GetSector(SPos pos) = 0;

  void SetWorld(World *world) { mWorld = world; }

  IMapGenerator & GetGenerator();


protected:
  World *mWorld = nullptr;
  std::unique_ptr<IMapGenerator> mGenerator;
};
