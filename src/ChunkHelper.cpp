#include "ChunkHelper.h"
#include "SectorCompiler.h"
#include "Sector.h"




void test()
{
//   IndexType block_pos = 0;
// 
//   static ChunkHelper helper;
//   static Around around;
//   SectorAround sector_around;
// 
// 
//   auto chunk = helper.chunk_info[helper.index_chunk_in_sector[block_pos]];
// 
//   // Ќаходим сектор и чанк дл€ указанной зоны.
//   for (size_t i = 0; i < around.middle.size(); ++i)
//   {
//     auto sector = sector_around.around[chunk.around[around.middle[i]].sector];
//     auto index_chunk = chunk.around[around.middle[i]].chunk;
// 
//     auto index_blocks = helper.chunk_info[index_chunk].blocks.middle[i];
//     auto index_tess = helper.chunk_info[index_chunk].tesselators.middle[i];
//   }
// 
//   for (size_t i = 0; i < around.neighbor_1.size(); ++i)
//   {
//     auto sector = sector_around.around[chunk.around[around.neighbor_1[i]].sector];
//     auto index_chunk = chunk.around[around.neighbor_1[i]].chunk;
// 
//     auto index_blocks = helper.chunk_info[index_chunk].blocks.neighbor_1[i];
//     auto index_tess = helper.chunk_info[index_chunk].tesselators.neighbor_1[i];
//   }
// 
//   int t1 = 1;
//   int t2 = 2;
//   std::reference_wrapper<int> k = t1;
//   k = t2;
// 
//   int j = 0;
}

Chunk::Chunk(const Sector &sector)
  : mSector(sector)
{

}

void Chunk::SetCompilerData(SectorCompiler &sectorCompiler)
{
  const auto &sectors = mSector.GetSectorAround().sectors;
  auto &tesselators = sectorCompiler.Tesselators();
  auto &tesselators_data = sectorCompiler.TesselatorsData();

  {
    const auto &sector = sectors[SectorAround::Middle].get();
    for (size_t i = 0; i < data.compilator_middle.size(); ++i)
    {
      tesselators[data.compilator_middle[i]] = sector.GetBlockId(i);
      if (sector.GetTesselatorData(i))
        tesselators_data[data.compilator_middle[i]] = *sector.GetTesselatorData(i);
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

const Chunk::Data Chunk::data;

const std::array<SPos, AROUND_CAPACITY> SectorAround::pos =
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

Chunk::Data::Data()
{
  IndexType i;
  
  // middle
  i = 0;
  for (SBPosType z = 0; z < gChunkSize.z(); ++z)
    for (SBPosType y = 0; y < gChunkSize.y(); ++y)
      for (SBPosType x = 0; x < gChunkSize.x(); ++x)
      {
        chunk_middle[i++] = cs::SBtoBI({ x, y, z });
      }

  i = 0;
  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
      for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
      {
        compilator_middle[i++] = cs::STtoTI({ x, y, z });
      }

  // east
  i = 0;
  for (SBPosType z = 0; z < gChunkSize.z(); ++z)
    for (SBPosType y = 0; y < gChunkSize.y(); ++y)
    {
      chunk_east[i++] = cs::SBtoBI({ gChunkSize.x() - 1, y, z });
    }
  i = 0;
  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
    {
      compilator_east[i++] = cs::STtoTI({ 0, y, z });
    }

  // west
  i = 0;
  for (SBPosType z = 0; z < gChunkSize.z(); ++z)
    for (SBPosType y = 0; y < gChunkSize.y(); ++y)
    {
      chunk_west[i++] = cs::SBtoBI({ 0, y, z });
    }
  i = 0;
  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
    {
      compilator_west[i++] = cs::STtoTI({ gBlockBatcherSize.x() - 1, y, z });
    }

  // South
  i = 0;
  for (SBPosType y = 0; y < gChunkSize.y(); ++y)
    for (SBPosType x = 0; x < gChunkSize.x(); ++x)
    {
      chunk_south[i++] = cs::SBtoBI({ x, y, gChunkSize.z() - 1 });
    }
  i = 0;
  for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
    for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
    {
      compilator_south[i++] = cs::STtoTI({ x, y, 0 });
    }

  // North
  i = 0;
  for (SBPosType y = 0; y < gChunkSize.y(); ++y)
    for (SBPosType x = 0; x < gChunkSize.x(); ++x)
    {
      chunk_north[i++] = cs::SBtoBI({ x, y, 0 });
    }
  i = 0;
  for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
    for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
    {
      compilator_north[i++] = cs::STtoTI({ x, y, gBlockBatcherSize.z() - 1 });
    }

  // Down
  i = 0;
  for (SBPosType z = 0; z < gChunkSize.z(); ++z)
    for (SBPosType x = 0; x < gChunkSize.x(); ++x)
    {
      chunk_down[i++] = cs::SBtoBI({ x, gChunkSize.y() - 1, z });
    }
  i = 0;
  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
    {
      compilator_down[i++] = cs::STtoTI({ x, 0, z });
    }

  // top
  i = 0;
  for (SBPosType z = 0; z < gChunkSize.z(); ++z)
    for (SBPosType x = 0; x < gChunkSize.x(); ++x)
    {
      chunk_top[i++] = cs::SBtoBI({ x, 0, z });
    }
  i = 0;
  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
    {
      compilator_top[i++] = cs::STtoTI({ x, gBlockBatcherSize.y() - 1, z });
    }
}
