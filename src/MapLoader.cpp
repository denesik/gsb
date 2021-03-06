#include "MapLoader.h"
#include "Sector.h"

#include <flatbuffers\flatbuffers.h>
#include <Sector_generated.h>
#include <fstream>
#include <boost/format.hpp>
#include <TesselatorMicroBlock.h>

MapLoaderFromGenerator::MapLoaderFromGenerator(const IMapGenerator &generator, const DataBase &db)
  : IMapLoader(db)
  , mGenerator(generator)
{

}

void MapLoaderFromGenerator::Process(Sector &sector)
{
  BlockId water = mDb.BlockIdFromName("water").value_or(1);

  auto &spos = sector.GetPos();
  auto &offset = cs::StoSB(spos);

  for (int i = 0; i < gSectorSize.x(); i++)
    for (int k = 0; k < gSectorSize.z(); k++)
    {
      const auto layering = mGenerator.GetLayering(mDb, offset.x() + i, offset.z() + k);
      const auto max_h = mGenerator.GetGroundLevel(mDb, offset.x() + i, offset.z() + k);
      const auto w_level = mGenerator.GetWaterLevel(mDb, offset.x() + i, offset.z() + k);

      if (w_level > max_h)
      {
        for (auto j = max_h; j < w_level; ++j)
        {
          sector.CreateBlock(SBPos{ i, j, k }, water);
        }
      }

      for (auto &bottom = layering.begin(); bottom != layering.end(); ++bottom)
      {
        for (auto j = bottom->first.lower(); j < bottom->first.upper() && j <= max_h; ++j)
          sector.CreateBlock(SBPos{ i, max_h - j, k }, bottom->second);
      }

      for (const auto &tess : layering.mTesselatorData)
      {
        auto &tessData = static_cast<TesselatorMicroBlock::Data &>(sector.GetTesselatorData(SBPos{ i, max_h - tess.first, k }).value());
        tessData = static_cast<TesselatorMicroBlock::Data &>(*tess.second);
      }
    }

  {
    std::list<MapTemplate> props;
    for (int it = -1; it <= 1; ++it)
      for (int jt = -1; jt <= 1; ++jt)
      {
        auto temp = mGenerator.GetProps(mDb, spos + SPos{ it, 0, jt });
        props.insert(props.begin(), temp.begin(), temp.end());
      }
    SBPos sec_min = cs::StoSB(spos);
    SBPos sec_max = sec_min + gSectorSize;
    for (const auto &p : props)
    {
      SBPos prop_min = p.position;
      SBPos prop_max = p.position + p.size;

      if (prop_max.x() < sec_min.x() || prop_max.y() < sec_min.y() || prop_max.z() < sec_min.z()
        || prop_min.x() >= sec_max.x() || prop_min.y() >= sec_max.y() || prop_min.z() >= sec_max.z())
        continue;

      for (int i = 0; i < p.size.x(); ++i)
      {
        if (prop_min.x() + i >= sec_max.x() || prop_min.x() + i < sec_min.x())
          continue;
        for (int j = 0; j < p.size.y(); ++j)
        {
          if (prop_min.y() + j >= sec_max.y() || prop_min.y() + j < sec_min.y())
            continue;
          for (int k = 0; k < p.size.z(); ++k)
          {
            if (prop_min.z() + k >= sec_max.z() || prop_min.z() + k < sec_min.z())
              continue;

            BlockId id = p.data[cs::SBtoBIcustom(SBPos(i, j, k), p.size)];
            if (!!id)
              sector.CreateBlock(prop_min + SBPos(i, j, k) - sec_min, id);
          }
        }
      }
    }
  }
}

MapLoaderFromDisk::MapLoaderFromDisk(boost::filesystem::path path, const DataBase & db)
  : IMapLoader(db)
  , mPath(path)
{
}

void MapLoaderFromDisk::Process(Sector &sector)
{
  
}

//MapLoaderFromNetwork::MapLoaderFromNetwork(boost::asio::ip::address address, const DataBase & db)
//  : IMapLoader(db)
//  , mAddress(address)
//{
//}
//
//void MapLoaderFromNetwork::Process()
//{
//}

// MapSaverToDisk::MapSaverToDisk(boost::filesystem::path path, const DataBase & db)
//   : IMapSaver(db)
//   , mPath(path)
// {
// }
// 
// void MapSaverToDisk::Process()
// {
//   flatbuffers::FlatBufferBuilder fbb;
//   auto pos = gsb_flat::SPos(mSector->mPos.x(), mSector->mPos.z());
//   auto blocks = fbb.CreateVector<int32_t>(reinterpret_cast<int32_t*>(&mSector->mStaticBlocks[0]), mSector->mStaticBlocks.size());
//   auto sector = gsb_flat::CreateSector(fbb, &pos, blocks);
//   fbb.Finish(sector);
// 
//   std::ofstream fs((boost::format("%1%.%2%.bin") % mSector->mPos.x() % mSector->mPos.z()).str(), std::ofstream::binary);
//   fs.write(reinterpret_cast<char *>(fbb.GetBufferPointer()), fbb.GetSize());
//   fs.close();
// }
