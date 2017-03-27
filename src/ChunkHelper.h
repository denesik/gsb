#pragma once
#ifndef ChunkHelper_h__
#define ChunkHelper_h__

#include <memory>
#include <array>
#include "tools\CoordSystem.h"

// —ектор состоит из чанков.
// „анки нужны дл€ обновлени€ света (сервер) и геометрии (клиент).
// ѕробегаем по чанкам в секторе. ƒл€ каждого чанка перестраиваем геометрию, если нужно.

enum
{
  T_SECTOR_SIZE = 16,
  T_SECTOR_HEIGHT = 256,

  CHUNK_SIZE = 8,
  CHUNK_CAPACITY = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE,

  CHUNK_TABLE_SIZE = T_SECTOR_SIZE / CHUNK_SIZE,
  CHUNK_TABLE_HEIGHT = T_SECTOR_HEIGHT / CHUNK_SIZE,
  CHUNK_TABLE_CAPACITY = CHUNK_TABLE_SIZE * CHUNK_TABLE_SIZE * CHUNK_TABLE_HEIGHT,

  AROUND_MIDDLE = 1,
  AROUND_NEIGHBOR_1 = 6,
  AROUND_NEIGHBOR_2 = 12,
  AROUND_NEIGHBOR_3 = 8,
  AROUND_CAPACITY = AROUND_MIDDLE + AROUND_NEIGHBOR_1 + AROUND_NEIGHBOR_2 + AROUND_NEIGHBOR_3,

  CHUNK_MIDDLE_CAPACITY = CHUNK_CAPACITY,
  CHUNK_NEIGHBOR_1_CAPACITY = CHUNK_SIZE * CHUNK_SIZE,
  CHUNK_NEIGHBOR_2_CAPACITY = CHUNK_SIZE,
  CHUNK_NEIGHBOR_3_CAPACITY = 1,

  T_CAPACITY =
  CHUNK_MIDDLE_CAPACITY * AROUND_MIDDLE +
  CHUNK_NEIGHBOR_1_CAPACITY * AROUND_NEIGHBOR_1 +
  CHUNK_NEIGHBOR_2_CAPACITY * AROUND_NEIGHBOR_2 +
  CHUNK_NEIGHBOR_3_CAPACITY * AROUND_NEIGHBOR_3,

};

class Sector;

struct SectorAround
{
  enum 
  {
    SouthEastDown,        // [0 0 0]
    SouthDown,            // [x 0 0]
    SouthWestDown,        // [1 0 0] 
    SouthEast,            // [0 y 0]
    South,                // [x y 0]
    SouthWest,            // [1 y 0] 
    SouthEastTop,         // [0 1 0]
    SouthTop,             // [x 1 0]
    SouthWestTop,         // [1 1 0] 

    EastDown,             // [0 0 z]
    Down,                 // [x 0 z]
    WestDown,             // [1 0 z]
    East,                 // [0 y z]
    Middle,               // [x y z]
    West,                 // [1 y z]
    EastTop,              // [0 1 z]
    Top,                  // [x 1 z]
    WestTop,              // [1 1 z]

    NorthEastDown,        // [0 0 1]
    NorthDown,            // [x 0 1]
    NorthWestDown,        // [1 0 1] 
    NorthEast,            // [0 y 1]
    North,                // [x y 1]
    NorthWest,            // [1 y 1] 
    NorthEastTop,         // [0 1 1]
    NorthTop,             // [x 1 1]
    NorthWestTop,         // [1 1 1] 

    AroundCapacity,
  };

  std::array<std::weak_ptr<Sector>, AroundCapacity> sectors;
  static const std::array<SPos, AroundCapacity> pos;
};

// »ндекс соседей вокруг источника в таблице соседей.
struct Around
{
  std::array<IndexType, AROUND_MIDDLE> middle;
  std::array<IndexType, AROUND_NEIGHBOR_1> neighbor_1;
  std::array<IndexType, AROUND_NEIGHBOR_2> neighbor_2;
  std::array<IndexType, AROUND_NEIGHBOR_3> neighbor_3;
};

// Ѕлоки внутри сектора и слой блоков толщиной 1 в соседних секторах.
struct BlockAround
{
  // »ндексы блоков в секторе дл€ текущего чанка.
  std::array<std::array<IndexType, CHUNK_MIDDLE_CAPACITY>, AROUND_MIDDLE> middle;

  // »ндексы блоков в секторе дл€ 1-го соседнего чанка.
  std::array<std::array<IndexType, CHUNK_NEIGHBOR_1_CAPACITY>, AROUND_NEIGHBOR_1> neighbor_1;

  // »ндексы блоков в секторе дл€ 2-х соседних чанков.
  std::array<std::array<IndexType, CHUNK_NEIGHBOR_2_CAPACITY>, AROUND_NEIGHBOR_2> neighbor_2;

  // »ндексы блоков в секторе дл€ 3-х соседних чанков.
  std::array<std::array<IndexType, CHUNK_NEIGHBOR_3_CAPACITY>, AROUND_NEIGHBOR_3> neighbor_3;
};

// »ндекс чанка и индекс сектора вокруг текущего чанка.
// ≈сли чанк в соседнем секторе, индекс определен дл€ чанка в соседнем секторе. 
struct ChunkAround
{
  IndexType sector;
  IndexType chunk;
};

struct ChunkInfo
{
  // “аблица индексов на сектора вокруг текущего чанка.
  std::array<ChunkAround, AROUND_CAPACITY> around;

  // »ндексы блоков дл€ зоны в центре которой текущий чанк.
  BlockAround blocks;

  // »ндексы тессел€торов дл€ зоны в центре которой текущий чанк.
  BlockAround tesselators;
};

// ѕри изменении блока в чанке, находим индекс этого чанка и стафим флаг об изменении чанка.
// ≈сли блок на границе чанков, ставим флаг об изменении у соседнего чанка.
// «апрашиваем дл€ каждого чанка в области нашего чанка сектор в котором находитс€ чанк.
// ѕолучаем даные, в которых каждому индексу чанка соответствует массив блоков.
// ƒл€ каждого индекса чанка запрашиваем список индексов на блоки.
// ƒл€ каждого индекса чанка запрашиваем список индексов на тессел€торы.
struct ChunkHelper
{
  // ƒл€ каждого блока в секторе определ€етс€ индекс чанка, в котором находитс€ блок.
  std::array<IndexType, gSectorCapacity> index_chunk_in_sector;

  std::array<ChunkInfo, CHUNK_TABLE_CAPACITY> chunk_info;
};

void test();


class Sector;
class SectorCompiler;

class Chunk
{
public:
  Chunk(const Sector &sector);

  void SetCompilerData(SectorCompiler &sectorCompiler);

private:
  const Sector &mSector;

  struct Data
  {
    Data();

    std::array<IndexType, gChunkCapacity> chunk_middle;
    std::array<IndexType, gChunkCapacity> compilator_middle;

    std::array<IndexType, gChunkSize.x() * gChunkSize.y()> chunk_south;
    std::array<IndexType, gChunkSize.x() * gChunkSize.z()> chunk_down;
    std::array<IndexType, gChunkSize.y() * gChunkSize.z()> chunk_east; 
    std::array<IndexType, gChunkSize.x() * gChunkSize.y()> chunk_north;
    std::array<IndexType, gChunkSize.x() * gChunkSize.z()> chunk_top;
    std::array<IndexType, gChunkSize.y() * gChunkSize.z()> chunk_west;

    std::array<IndexType, gChunkSize.x() * gChunkSize.y()> compilator_south;
    std::array<IndexType, gChunkSize.x() * gChunkSize.z()> compilator_down;
    std::array<IndexType, gChunkSize.y() * gChunkSize.z()> compilator_east; 
    std::array<IndexType, gChunkSize.x() * gChunkSize.y()> compilator_north;
    std::array<IndexType, gChunkSize.x() * gChunkSize.z()> compilator_top;
    std::array<IndexType, gChunkSize.y() * gChunkSize.z()> compilator_west;
  };

  static const Data data;
};


#endif // ChunkHelper_h__
