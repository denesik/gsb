#include "IMapLoader.h"
#include "World.h"

IMapLoader::IMapLoader()
{
}

IMapLoader::~IMapLoader()
{
}

std::shared_ptr<Sector> IMapLoader::GetSector(SPos pos)
{
  return {};
}

void IMapLoader::SetWorld(World * world)
{
  mWorld = world;
}
