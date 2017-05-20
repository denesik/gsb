#include "ChunkHelper.h"
#include "SectorCompiler.h"
#include "Sector.h"


Chunk::Chunk(const Sector &sector)
  : mSector(sector)
{

}

void Chunk::SetCompilerData(SectorCompiler &sectorCompiler)
{
  const auto &sectors = mSector.GetSectorAround().sectors;
  auto &tesselators = sectorCompiler.Tesselators();
  auto &tesselators_data = sectorCompiler.TesselatorsData();

  sectorCompiler.SetPos(data.pos);

  {
    const auto &sector = sectors[SectorAround::Middle].get();
    for (size_t i = 0; i < data.compilator_middle.size(); ++i)
    {
      tesselators[data.compilator_middle[i]] = sector.GetBlockId(data.chunk_middle[i]);
      if (sector.GetTesselatorData(data.chunk_middle[i]))
        tesselators_data[data.compilator_middle[i]] = *sector.GetTesselatorData(data.chunk_middle[i]);
    }
  }

  {
    const auto &sector = sectors[SectorAround::South].get();
    for (size_t i = 0; i < data.compilator_south.size(); ++i)
    {
      tesselators[data.compilator_south[i]] = sector.GetBlockId(data.chunk_south[i]);
      if (sector.GetTesselatorData(data.chunk_south[i]))
        tesselators_data[data.compilator_south[i]] = *sector.GetTesselatorData(data.chunk_south[i]);
    }
  }

  {
    const auto &sector = sectors[SectorAround::Down].get();
    for (size_t i = 0; i < data.compilator_down.size(); ++i)
    {
      tesselators[data.compilator_down[i]] = sector.GetBlockId(data.chunk_down[i]);
      if (sector.GetTesselatorData(data.chunk_down[i]))
        tesselators_data[data.compilator_down[i]] = *sector.GetTesselatorData(data.chunk_down[i]);
    }
  }

  {
    const auto &sector = sectors[SectorAround::East].get();
    for (size_t i = 0; i < data.compilator_east.size(); ++i)
    {
      tesselators[data.compilator_east[i]] = sector.GetBlockId(data.chunk_east[i]);
      if (sector.GetTesselatorData(data.chunk_east[i]))
        tesselators_data[data.compilator_east[i]] = *sector.GetTesselatorData(data.chunk_east[i]);
    }
  }

  {
    const auto &sector = sectors[SectorAround::North].get();
    for (size_t i = 0; i < data.compilator_north.size(); ++i)
    {
      tesselators[data.compilator_north[i]] = sector.GetBlockId(data.chunk_north[i]);
      if (sector.GetTesselatorData(data.chunk_north[i]))
        tesselators_data[data.compilator_north[i]] = *sector.GetTesselatorData(data.chunk_north[i]);
    }
  }

  {
    const auto &sector = sectors[SectorAround::Top].get();
    for (size_t i = 0; i < data.compilator_top.size(); ++i)
    {
      tesselators[data.compilator_top[i]] = sector.GetBlockId(data.chunk_top[i]);
      if (sector.GetTesselatorData(data.chunk_top[i]))
        tesselators_data[data.compilator_top[i]] = *sector.GetTesselatorData(data.chunk_top[i]);
    }
  }

  {
    const auto &sector = sectors[SectorAround::West].get();
    for (size_t i = 0; i < data.compilator_west.size(); ++i)
    {
      tesselators[data.compilator_west[i]] = sector.GetBlockId(data.chunk_west[i]);
      if (sector.GetTesselatorData(data.chunk_west[i]))
        tesselators_data[data.compilator_west[i]] = *sector.GetTesselatorData(data.chunk_west[i]);
    }
  }
}

const std::array<SPos, SectorAround::AroundCapacity> SectorAround::pos =
{
  cs::South(cs::East(cs::Down(SPos()))),
  cs::South(cs::Down(SPos())),
  cs::South(cs::West(cs::Down(SPos()))),
  cs::South(cs::East(SPos())),
  cs::South(SPos()),
  cs::South(cs::West(SPos())),
  cs::South(cs::East(cs::Top(SPos()))),
  cs::South(cs::Top(SPos())),
  cs::South(cs::West(cs::Top(SPos()))),

  cs::East(cs::Down(SPos())),
  cs::Down(SPos()),
  cs::West(cs::Down(SPos())),
  cs::East(SPos()),
  SPos(),
  cs::West(SPos()),
  cs::East(cs::Top(SPos())),
  cs::Top(SPos()),
  cs::West(cs::Top(SPos())),

  cs::North(cs::East(cs::Down(SPos()))),
  cs::North(cs::Down(SPos())),
  cs::North(cs::West(cs::Down(SPos()))),
  cs::North(cs::East(SPos())),
  cs::North(SPos()),
  cs::North(cs::West(SPos())),
  cs::North(cs::East(cs::Top(SPos()))),
  cs::North(cs::Top(SPos())),
  cs::North(cs::West(cs::Top(SPos()))),
};

const Chunk::Data Chunk::data(SPos{ 0, 0, 0 });

Chunk::Data::Data(const SPos &_pos)
  : pos(_pos)
{
  IndexType i;
  
  // middle
  i = 0;
  for (SBPosType z = 0; z < gChunkSize.z(); ++z)
    for (SBPosType y = 0; y < gChunkSize.y(); ++y)
      for (SBPosType x = 0; x < gChunkSize.x(); ++x)
      {
        chunk_middle[i++] = cs::SBtoBI(SPos(x, y, z) + gChunkSize * pos);
      }

  i = 0;
  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
      for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
      {
        compilator_middle[i++] = cs::STtoTI(SPos(x, y, z));
      }


  // east
  i = 0;
  for (SBPosType z = 0; z < gChunkSize.z(); ++z)
    for (SBPosType y = 0; y < gChunkSize.y(); ++y)
    {
      chunk_east[i++] = cs::SBtoBI(SPos(gChunkSize.x() - 1, y, z) + gChunkSize * pos);
    }
  i = 0;
  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
    {
      compilator_east[i++] = cs::STtoTI(SPos(0, y, z));
    }

  // west
  i = 0;
  for (SBPosType z = 0; z < gChunkSize.z(); ++z)
    for (SBPosType y = 0; y < gChunkSize.y(); ++y)
    {
      chunk_west[i++] = cs::SBtoBI(SPos(0, y, z) + gChunkSize * pos);
    }
  i = 0;
  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
    {
      compilator_west[i++] = cs::STtoTI(SPos(gBlockBatcherSize.x() - 1, y, z));
    }

  // South
  i = 0;
  for (SBPosType y = 0; y < gChunkSize.y(); ++y)
    for (SBPosType x = 0; x < gChunkSize.x(); ++x)
    {
      chunk_south[i++] = cs::SBtoBI(SPos(x, y, gChunkSize.z() - 1) + gChunkSize * pos);
    }
  i = 0;
  for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
    for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
    {
      compilator_south[i++] = cs::STtoTI(SPos(x, y, 0));
    }

  // North
  i = 0;
  for (SBPosType y = 0; y < gChunkSize.y(); ++y)
    for (SBPosType x = 0; x < gChunkSize.x(); ++x)
    {
      chunk_north[i++] = cs::SBtoBI(SPos(x, y, 0) + gChunkSize * pos);
    }
  i = 0;
  for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
    for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
    {
      compilator_north[i++] = cs::STtoTI(SPos(x, y, gBlockBatcherSize.z() - 1));
    }

  // Down
  i = 0;
  for (SBPosType z = 0; z < gChunkSize.z(); ++z)
    for (SBPosType x = 0; x < gChunkSize.x(); ++x)
    {
      chunk_down[i++] = cs::SBtoBI(SPos(x, gChunkSize.y() - 1, z) + gChunkSize * pos);
    }
  i = 0;
  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
    {
      compilator_down[i++] = cs::STtoTI(SPos(x, 0, z));
    }

  // top
  i = 0;
  for (SBPosType z = 0; z < gChunkSize.z(); ++z)
    for (SBPosType x = 0; x < gChunkSize.x(); ++x)
    {
      chunk_top[i++] = cs::SBtoBI(SPos(x, 0, z) + gChunkSize * pos);
    }
  i = 0;
  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
    {
      compilator_top[i++] = cs::STtoTI(SPos(x, gBlockBatcherSize.y() - 1, z));
    }
}
