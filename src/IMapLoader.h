#pragma once
#include "tools/CoordSystem.h"
#include <memory>
#include <tools/Common.h>

class Sector;
class World;

class GSB_NOVTABLE IMapLoader
{
public:
  virtual ~IMapLoader() = default;
  virtual std::shared_ptr<Sector> GetSector(SPos pos) = 0;

  void SetWorld(World *world) { mWorld = world; }

protected:
  World *mWorld = nullptr;
};
