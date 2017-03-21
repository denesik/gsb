#include "ChunkHelper.h"




void test()
{
  IndexType block_pos = 0;

  static ChunkHelper helper;
  static Around around;
  SectorAround sector_around;


  auto chunk = helper.chunk_info[helper.index_chunk_in_sector[block_pos]];

  // Ќаходим сектор и чанк дл€ указанной зоны.
  for (size_t i = 0; i < around.middle.size(); ++i)
  {
    auto sector = sector_around.around[chunk.around[around.middle[i]].sector];
    auto index_chunk = chunk.around[around.middle[i]].chunk;

    auto index_blocks = helper.chunk_info[index_chunk].blocks.middle[i];
    auto index_tess = helper.chunk_info[index_chunk].tesselators.middle[i];
  }

  for (size_t i = 0; i < around.neighbor_1.size(); ++i)
  {
    auto sector = sector_around.around[chunk.around[around.neighbor_1[i]].sector];
    auto index_chunk = chunk.around[around.neighbor_1[i]].chunk;

    auto index_blocks = helper.chunk_info[index_chunk].blocks.neighbor_1[i];
    auto index_tess = helper.chunk_info[index_chunk].tesselators.neighbor_1[i];
  }

  int t1 = 1;
  int t2 = 2;
  std::reference_wrapper<int> k = t1;
  k = t2;

  int j = 0;
}
