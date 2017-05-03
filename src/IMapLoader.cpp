#include "IMapLoader.h"

IMapLoader::IMapLoader(const DataBase &db)
  : mDb(db)
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

IMapSaver::IMapSaver(const DataBase & db)
  : mDb(db)
{
}

IMapSaver::~IMapSaver()
{
}

void IMapSaver::SetSector(Sector * sector)
{
  mSector = sector;
}
