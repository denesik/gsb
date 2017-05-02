#include "IMapLoader.h"

IMapLoader::IMapLoader(const IMapGenerator &generator, const DataBase &db)
  : mGenerator(generator)
  , mDb(db)
{

}

IMapLoader::~IMapLoader()
{
  Release();
}

void IMapLoader::SetSector(std::weak_ptr<Sector> sector)
{
  mSector = sector;
}
