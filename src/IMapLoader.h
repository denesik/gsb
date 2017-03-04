#pragma once
#include "tools/CoordSystem.h"
#include <memory>

class Sector;

class IMapLoader
{
public:
  virtual ~IMapLoader() = default;
  virtual std::weak_ptr<Sector> GetSector(SPos pos) = 0;
};
