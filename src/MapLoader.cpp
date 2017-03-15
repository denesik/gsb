#include "MapLoader.h"
#include "Sector.h"


MapLoader::MapLoader(const IMapGenerator &generator)
  : mGenerator(generator)
{

}

MapLoader::~MapLoader()
{
  Release();
}

void MapLoader::SetSector(std::weak_ptr<Sector> sector)
{
  mSector = sector;
}


void MapLoader::Process()
{
  if (!mSector.expired())
  {
    mGenerator.Generate(*mSector.lock());
  }
}
