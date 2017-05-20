#pragma once
#ifndef ChunkHelper_h__
#define ChunkHelper_h__

#include <memory>
#include <array>
#include "tools\CoordSystem.h"

// —ектор состоит из чанков.
// „анки нужны дл€ обновлени€ света (сервер) и геометрии (клиент).
// ѕробегаем по чанкам в секторе. ƒл€ каждого чанка перестраиваем геометрию, если нужно.


namespace tools
{
  namespace detail
  {

    template <typename T, std::size_t...Is>
    std::array<T, sizeof...(Is)> make_array(const T& value, std::index_sequence<Is...>)
    {
      return{ { (static_cast<void>(Is), value)... } };
    }
  }

  template <std::size_t N, typename T>
  std::array<T, N> make_array(const T& value)
  {
    return detail::make_array(value, std::make_index_sequence<N>());
  }
}

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

  SectorAround(Sector &sector)
    : sectors(tools::make_array<AroundCapacity>(std::ref(sector)))
  {}

  std::array<std::reference_wrapper<Sector>, AroundCapacity> sectors;
  static const std::array<SPos, AroundCapacity> pos;

  inline bool inside(const SPos &pos)
  {
    return pos >= SPos(-1) && pos <= SPos(1);
  }

  inline IndexType to_index(const SPos &pos)
  {
    return static_cast<IndexType>(pos.z() + 1) * 3 * 3 +
      static_cast<IndexType>(pos.y() + 1) * 3 +
      static_cast<IndexType>(pos.x() + 1);
  }
};

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
    Data(const SPos &pos);

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

    SPos pos;
  };

  static const Data data;
};


#endif // ChunkHelper_h__
