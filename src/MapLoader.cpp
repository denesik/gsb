#include "MapLoader.h"
#include "Sector.h"


MapLoader::MapLoader(const IMapGenerator &generator, const DataBase &db)
  : mGenerator(generator)
  , mDb(db)
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
    auto &sector = mSector.lock();
    auto &spos = sector->GetPos();
    auto &offset = cs::StoSB(spos);
    for (int i = 0; i < gSectorSize.x(); i++)
      for (int k = 0; k < gSectorSize.z(); k++)
      {
        const auto &layering = mGenerator.GetLayering(mDb, offset.x() + i, offset.z() + k);
        for(auto &bottom = layering.begin(); bottom != layering.end(); ++bottom)
        {
          for(auto j = bottom->first.lower(); j < bottom->first.upper() && j < gSectorSize.y(); ++j)
            sector->CreateBlock(SBPos{i, j, k}, bottom->second);
        }
      }
  }
}
