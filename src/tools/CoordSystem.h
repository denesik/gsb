// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef CoordsConvert_h__
#define CoordsConvert_h__

#include <Magnum/Math/Vector3.h>
#include <Magnum/Magnum.h>
#include <assert.h>

enum
{
  SECTOR_SIZE = 32,
  SECTOR_CAPACITY = SECTOR_SIZE * SECTOR_SIZE * SECTOR_SIZE,

  TESSELATOR_SIZE = SECTOR_SIZE + 2,
  TESSELATOR_CAPACITY = TESSELATOR_SIZE * TESSELATOR_SIZE * TESSELATOR_SIZE,

  SECTOR_COUNT_HEIGHT = 8,

  MICROBLOCK_SIZE = 4,
  MICROBLOCK_CAPACITY = MICROBLOCK_SIZE * MICROBLOCK_SIZE * MICROBLOCK_SIZE,
};

enum SideFlags : int
{
  NONE = 0,

  FRONT = 1 << 0,
  RIGHT = 1 << 1,
  BACK = 1 << 2,
  LEFT = 1 << 3,
  TOP = 1 << 4,
  BOTTOM = 1 << 5,

  ALL = 0x3F,
};

inline int SideFlagIndex(SideFlags side)
{
  for (int i = 0; i < 6; ++i)
  {
    if (side & (1 << i))
      return i;
  }

  return 0;
}

using CSPos     = Magnum::Vector2i; ///  оординаты столбца секторов
using WPos      = Magnum::Vector3;  /// ћировые координаты.
using SPos      = Magnum::Vector3i; ///  оординаты сектора.
using WBPos     = Magnum::Vector3i; ///  оординаты блока в мире.
using SBPos     = Magnum::Vector3i; ///  оординаты блока в секторе.
using STPos     = Magnum::Vector3i; ///  оординаты тессел€тора в секторе.
using WPosType  = Magnum::Float;
using SPosType  = Magnum::Int;
using WBPosType = Magnum::Int;
using SBPosType = Magnum::Int;
using STPosType = Magnum::Int;
using IndexType = Magnum::UnsignedInt;

/// —истема координат.
namespace cs
{
  namespace detail
  {
    template<class T>
    inline float move(float val)
    {
      return val + static_cast<T> (1.0f - (static_cast<T> (val) - val));
    }
  }

  /// ћировые координаты в координаты блока в мире.
  inline WBPos WtoWB(const WPos &pos)
  {
    WBPos wbpos;

    wbpos.x() = (pos.x() >= 0) ? static_cast<WBPosType>(pos.x()) : static_cast<WBPosType>(detail::move<WBPosType>(pos.x())) - WBPosType(1);
    wbpos.y() = (pos.y() >= 0) ? static_cast<WBPosType>(pos.y()) : static_cast<WBPosType>(detail::move<WBPosType>(pos.y())) - WBPosType(1);
    wbpos.z() = (pos.z() >= 0) ? static_cast<WBPosType>(pos.z()) : static_cast<WBPosType>(detail::move<WBPosType>(pos.z())) - WBPosType(1);

    return wbpos;
  }

  ///  оординаты блока в мире в координаты сектора.
  inline SPos WBtoS(const WBPos &pos)
  {
    SPos spos;
    const SPosType size = static_cast<SPosType>(SECTOR_SIZE);

    spos.x() = (pos.x() >= 0) ? static_cast<SPosType>(pos.x()) / size : (static_cast<SPosType>(pos.x()) - size + SPosType(1)) / size;
    spos.y() = (pos.y() >= 0) ? static_cast<SPosType>(pos.y()) / size : (static_cast<SPosType>(pos.y()) - size + SPosType(1)) / size;
    spos.z() = (pos.z() >= 0) ? static_cast<SPosType>(pos.z()) / size : (static_cast<SPosType>(pos.z()) - size + SPosType(1)) / size;

    return spos;
  }

  /// ћировые координаты в координаты сектора.
  inline SPos WtoS(const WPos &pos)
  {
    return WBtoS(WtoWB(pos));
  }

  /// ћировые координаты в координаты блока в секторе.
  inline SBPos WtoSB(const WPos &pos)
  {
    auto wbpos = WtoWB(pos);
    auto spos = WBtoS(wbpos);
    const SPosType size = static_cast<SPosType>(SECTOR_SIZE);

    return wbpos - spos * size;
  }

  /// ћировые координаты в координаты блока в секторе.
  inline SBPos WtoSB(const WPos &pos, const SPos &spos)
  {
    const SBPosType size = static_cast<SBPosType>(SECTOR_SIZE);

    return WtoWB(pos) - spos * size;
  }

  ///  оординаты блока в мире в координаты блока в секторе.
  inline SBPos WBtoSB(const WBPos &pos)
  {
    auto spos = WBtoS(pos);
    const SPosType size = static_cast<SPosType>(SECTOR_SIZE);

    return pos - spos * size;
  }

  ///  оординаты блока в мире в координаты блока в секторе.
  inline SBPos WBtoSB(const WBPos &pos, const SPos &spos)
  {
    const SBPosType size = static_cast<SBPosType>(SECTOR_SIZE);

    return pos - spos * size;
  }

  ///  оординаты блока в секторе в координаты блока в мире.
  inline WBPos SBtoWB(const SBPos &pos, const SPos &spos)
  {
    const WBPosType size = static_cast<WBPosType>(SECTOR_SIZE);

    return spos * size + pos;
  }

  ///  оординаты сектора в мировые координаты.
  /// ¬озвращаютс€ координаты нулевого блока в секторе.
  inline WPos StoW(const SPos &pos)
  {
    WPos wpos(pos);
    const WPosType size = static_cast<WPosType>(SECTOR_SIZE);

    return wpos * size;
  }

  /// »ндекс блока в секторе в позицию блока в секторе.
  inline SBPos BItoSB(IndexType i)
  {
    const IndexType size = static_cast<IndexType>(SECTOR_SIZE);
    return SBPos{ 
      static_cast<SBPosType>(i % size), 
      static_cast<SBPosType>((i / size) % size), 
      static_cast<SBPosType>(i / (size * size)) };
  }

  /// ѕозици€ блока в секторе в индекс блока в секторе.
  inline IndexType SBtoBI(const SBPos &pos)
  {
    const IndexType size = static_cast<IndexType>(SECTOR_SIZE);
    return static_cast<IndexType>(pos.z()) * size * size +
           static_cast<IndexType>(pos.y()) * size +
           static_cast<IndexType>(pos.x());
  }

  /// »ндекс тессел€тора в секторе в позицию тессел€тора в секторе.
  inline STPos TItoST(IndexType i)
  {
    const IndexType size = static_cast<IndexType>(SECTOR_SIZE);
    return STPos{
      static_cast<STPosType>(i % size),
      static_cast<STPosType>((i / size) % size),
      static_cast<STPosType>(i / (size * size)) };
  }

  /// ѕозици€ тессел€тора в секторе в индекс тессел€тора в секторе.
  inline IndexType STtoTI(const STPos &pos)
  {
    const IndexType size = static_cast<IndexType>(TESSELATOR_SIZE);
    return static_cast<IndexType>(pos.z()) * size * size +
           static_cast<IndexType>(pos.y()) * size +
           static_cast<IndexType>(pos.x());
  }

  /// »ндекс тессел€тора в секторе в позицию блока в секторе.
  inline SBPos TItoSB(IndexType i)
  {
    const IndexType size = static_cast<IndexType>(TESSELATOR_SIZE);
    assert(i % size > 0 &&
      (i / size) % size > 0 &&
      i / (size * size) > 0);
    return SBPos{
      static_cast<SBPosType>(i % size - 1),
      static_cast<SBPosType>((i / size) % size - 1),
      static_cast<SBPosType>(i / (size * size) - 1) };
  }

  inline SBPos Left(const SBPos &pos)
  {
    return{ pos.x() + 1, pos.y(), pos.z() };
  }

  inline SBPos Right(const SBPos &pos)
  {
    return{ pos.x() - 1, pos.y(), pos.z() };
  }

  inline SBPos Top(const SBPos &pos)
  {
    return{ pos.x(), pos.y() + 1, pos.z() };
  }

  inline SBPos Bottom(const SBPos &pos)
  {
    return{ pos.x(), pos.y() - 1, pos.z() };
  }
  inline SBPos Back(const SBPos &pos)
  {
    return{ pos.x(), pos.y(), pos.z() + 1 };
  }

  inline SBPos Front(const SBPos &pos)
  {
    return{ pos.x(), pos.y(), pos.z() - 1 };
  }
}

namespace std
{
  template <>
  struct hash<SPos>
  {
    std::size_t operator()(SPos const& v) const
    {
      std::size_t h1 = std::hash<SPosType>()(v.x());
      std::size_t h2 = std::hash<SPosType>()(v.y());
      std::size_t h3 = std::hash<SPosType>()(v.z());
      return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
  };
}

#endif // CoordsConvert_h__