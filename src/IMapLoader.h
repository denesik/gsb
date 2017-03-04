#pragma once
#include "tools/CoordSystem.h"
#include <memory>

class Sector;
class World;

class IMapLoader
{
public:
  IMapLoader();
  virtual ~IMapLoader();
  virtual std::shared_ptr<Sector> GetSector(SPos pos);
  void SetWorld(World *world);

protected:
  World *mWorld;
};
