#pragma once
#ifndef ChunkHelper_h__
#define ChunkHelper_h__

#include <memory>
#include <array>
#include "tools\CoordSystem.h"

enum
{
  CHUNK_SIZE = 8,
  CHUNK_CAPACITY = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE,
  CHUNK_TABLE_SIZE = SECTOR_SIZE / CHUNK_SIZE,
  CHUNK_TABLE_HEIGHT = SECTOR_HEIGHT / CHUNK_SIZE,
  CHUNK_TABLE_CAPACITY = CHUNK_TABLE_SIZE * CHUNK_TABLE_SIZE * CHUNK_TABLE_HEIGHT,

  CHUNK_INFO_MIDDLE = 1,
  CHUNK_INFO_NEIGHBOR_1 = 6,
  CHUNK_INFO_NEIGHBOR_2 = 12,
  CHUNK_INFO_NEIGHBOR_3 = 8,
  CHUNK_INFO_CAPACITY = CHUNK_INFO_MIDDLE + CHUNK_INFO_NEIGHBOR_1 + CHUNK_INFO_NEIGHBOR_2 + CHUNK_INFO_NEIGHBOR_3,

  CHUNK_INFO_MIDDLE_CAPACITY = CHUNK_CAPACITY,
  CHUNK_INFO_NEIGHBOR_1_CAPACITY = CHUNK_SIZE * CHUNK_SIZE,
  CHUNK_INFO_NEIGHBOR_2_CAPACITY = CHUNK_SIZE,
  CHUNK_INFO_NEIGHBOR_3_CAPACITY = 1,

  T_CAPACITY =
  CHUNK_INFO_MIDDLE_CAPACITY * CHUNK_INFO_MIDDLE +
  CHUNK_INFO_NEIGHBOR_1_CAPACITY * CHUNK_INFO_NEIGHBOR_1 +
  CHUNK_INFO_NEIGHBOR_2_CAPACITY * CHUNK_INFO_NEIGHBOR_2 +
  CHUNK_INFO_NEIGHBOR_3_CAPACITY * CHUNK_INFO_NEIGHBOR_3,

};

class Sector;

struct SectorAround
{
  enum 
  {

  };
  std::weak_ptr<Sector> area[CHUNK_INFO_CAPACITY];
};

struct ChunkAround
{
  std::array<IndexType, CHUNK_INFO_MIDDLE> index_middle;
  std::array<IndexType, CHUNK_INFO_NEIGHBOR_1> index_neighboar_1;
  std::array<IndexType, CHUNK_INFO_NEIGHBOR_2> index_neighboar_2;
  std::array<IndexType, CHUNK_INFO_NEIGHBOR_3> index_neighboar_3;
};

struct BlockAround
{
  // Индексы блоков в секторе для текущего чанка.
  std::array<std::array<IndexType, CHUNK_INFO_MIDDLE_CAPACITY>, CHUNK_INFO_MIDDLE> middle;

  // Индексы блоков в секторе для 1-го соседнего чанка.
  std::array<std::array<IndexType, CHUNK_INFO_NEIGHBOR_1_CAPACITY>, CHUNK_INFO_NEIGHBOR_1> neighbor_1;

  // Индексы блоков в секторе для 2-х соседних чанков.
  std::array<std::array<IndexType, CHUNK_INFO_NEIGHBOR_2_CAPACITY>, CHUNK_INFO_NEIGHBOR_2> neighbor_2;

  // Индексы блоков в секторе для 3-х соседних чанков.
  std::array<std::array<IndexType, CHUNK_INFO_NEIGHBOR_3_CAPACITY>, CHUNK_INFO_NEIGHBOR_3> neighbor_3;
};

struct ChunkInfo
{
  // Таблица индексов на сектора вокруг текущего чанка.
  std::array<IndexType, CHUNK_INFO_CAPACITY> index_sector_around;

  // Индексы чанков вокруг текущего чанка.
  // Если чанк в соседнем секторе, индекс определен для чанка в соседнем секторе. 
  std::array<IndexType, CHUNK_INFO_CAPACITY> index_chunk_around;

  // Индексы блоков для зоны в центре которой текущий чанк.
  BlockAround blocks;

  // Индексы тесселяторов для зоны в центре которой текущий чанк.
  BlockAround tesselators;
};

// При изменении блока в чанке, находим индекс этого чанка и стафим флаг об изменении чанка.
// Если блок на границе чанков, ставим флаг об изменении у соседнего чанка.
// Запрашиваем для каждого чанка в области нашего чанка сектор в котором находится чанк.
// Получаем даные, в которых каждому индексу чанка соответствует массив блоков.
// Для каждого индекса чанка запрашиваем список индексов на блоки.
// Для каждого индекса чанка запрашиваем список индексов на тесселяторы.
struct ChunkHelper
{
  // Для каждого блока в секторе определяется индекс чанка, в котором находится блок.
  std::array<IndexType, SECTOR_CAPACITY> index_chunk_in_sector;

  std::array<ChunkInfo, CHUNK_TABLE_CAPACITY> chunk_info;
};

#endif // ChunkHelper_h__
