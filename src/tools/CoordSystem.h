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

enum SideIndex : int
{
  ISOUTH = 0,   // юг
  IEAST = 1,    // восток
  INORTH = 2,   // север
  IWEST = 3,    // запад
  ITOP = 4,     
  IDOWN = 5,
};

enum SideFlags : int
{
  NONE = 0,

  SOUTH = 1 << ISOUTH, 
  EAST = 1 << IEAST, 
  NORTH = 1 << INORTH,
  WEST = 1 << IWEST,
  TOP = 1 << ITOP,
  DOWN = 1 << IDOWN,

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


constexpr SPos gSectorSize(16, 256, 16);
constexpr Magnum::Int gSectorCapacity = gSectorSize.x() * gSectorSize.y() * gSectorSize.z();

constexpr SPos gChunkSize(gSectorSize.x(), gSectorSize.y(), gSectorSize.z());
constexpr Magnum::Int gChunkCapacity = gChunkSize.x() * gChunkSize.y() * gChunkSize.z();

constexpr SPos gBlockBatcherSize(gChunkSize.x() + 2, gChunkSize.y() + 2, gChunkSize.z() + 2);
constexpr Magnum::Int gBlockBatcherCapacity = gBlockBatcherSize.x() * gBlockBatcherSize.y() * gBlockBatcherSize.z();

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

    spos.x() = (pos.x() >= 0) ? static_cast<SPosType>(pos.x()) / gSectorSize.x() : (static_cast<SPosType>(pos.x()) - gSectorSize.x() + SPosType(1)) / gSectorSize.x();
    spos.y() = (pos.y() >= 0) ? static_cast<SPosType>(pos.y()) / gSectorSize.y() : (static_cast<SPosType>(pos.y()) - gSectorSize.y() + SPosType(1)) / gSectorSize.y();
    spos.z() = (pos.z() >= 0) ? static_cast<SPosType>(pos.z()) / gSectorSize.z() : (static_cast<SPosType>(pos.z()) - gSectorSize.z() + SPosType(1)) / gSectorSize.z();

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
    return wbpos - spos * gSectorSize;
  }

  /// ћировые координаты в координаты блока в секторе.
  inline SBPos WtoSB(const WPos &pos, const SPos &spos)
  {
    return WtoWB(pos) - spos * gSectorSize;
  }

  ///  оординаты блока в мире в координаты блока в секторе.
  inline SBPos WBtoSB(const WBPos &pos)
  {
    auto spos = WBtoS(pos);
    return pos - spos * gSectorSize;
  }

  ///  оординаты блока в мире в координаты блока в секторе.
  inline SBPos WBtoSB(const WBPos &pos, const SPos &spos)
  {
    return pos - spos * gSectorSize;
  }

  ///  оординаты блока в секторе в координаты блока в мире.
  inline WBPos SBtoWB(const SBPos &pos, const SPos &spos)
  {
    return spos * gSectorSize + pos;
  }

  ///  оординаты сектора в мировые координаты.
  /// ¬озвращаютс€ координаты нулевого блока в секторе.
  inline WPos StoW(const SPos &pos)
  {
    WPos wpos(pos * gSectorSize);
    return wpos;
  }

  /// »ндекс блока в секторе в позицию блока в секторе.
  inline SBPos BItoSB(IndexType i)
  {
    return SBPos{ 
      static_cast<SBPosType>(i % gSectorSize.x()),
      static_cast<SBPosType>((i / gSectorSize.x()) % gSectorSize.y()),
      static_cast<SBPosType>(i / (gSectorSize.x() * gSectorSize.y())) };
  }

  /// ѕозици€ блока в секторе в индекс блока в секторе.
  inline IndexType SBtoBI(const SBPos &pos)
  {
    return static_cast<IndexType>(pos.z()) * gSectorSize.x() * gSectorSize.y() +
           static_cast<IndexType>(pos.y()) * gSectorSize.x() +
           static_cast<IndexType>(pos.x());
  }

  /// »ндекс тессел€тора в секторе в позицию тессел€тора в секторе.
  inline STPos TItoST(IndexType i)
  {
    return SBPos{ 
      static_cast<SBPosType>(i % gSectorSize.x()),
      static_cast<SBPosType>((i / gSectorSize.x()) % gSectorSize.y()),
      static_cast<SBPosType>(i / (gSectorSize.x() * gSectorSize.y())) };
  }

  /// ѕозици€ тессел€тора в секторе в индекс тессел€тора в секторе.
  inline IndexType STtoTI(const STPos &pos)
  {
    return static_cast<IndexType>(pos.z()) * gBlockBatcherSize.x() * gBlockBatcherSize.y() +
      static_cast<IndexType>(pos.y()) * gBlockBatcherSize.x() +
      static_cast<IndexType>(pos.x());
  }

  /// »ндекс тессел€тора в секторе в позицию блока в секторе.
  inline SBPos TItoSB(IndexType i)
  {
    assert(i % gBlockBatcherSize.x() > 0 &&
      (i / gBlockBatcherSize.x()) % gBlockBatcherSize.y() > 0 &&
      i / (gBlockBatcherSize.x() * gBlockBatcherSize.y()) > 0);
    return SBPos{
      static_cast<SBPosType>(i % gBlockBatcherSize.x() - 1),
      static_cast<SBPosType>((i / gBlockBatcherSize.x()) % gBlockBatcherSize.y() - 1),
      static_cast<SBPosType>(i / (gBlockBatcherSize.x() * gBlockBatcherSize.y()) - 1) };
  }

  inline SBPos South(const SBPos &pos, SBPosType dist = SBPosType(1))
  {
    return{ pos.x(), pos.y(), pos.z() - dist };
  }

  inline SBPos East(const SBPos &pos, SBPosType dist = SBPosType(1))
  {
    return{ pos.x() - dist, pos.y(), pos.z() };
  }

  inline SBPos North(const SBPos &pos, SBPosType dist = SBPosType(1))
  {
    return{ pos.x(), pos.y(), pos.z() + dist };
  }

  inline SBPos West(const SBPos &pos, SBPosType dist = SBPosType(1))
  {
    return{ pos.x() + dist, pos.y(), pos.z() };
  }

  inline SBPos Top(const SBPos &pos, SBPosType dist = SBPosType(1))
  {
    return{ pos.x(), pos.y() + dist, pos.z() };
  }

  inline SBPos Down(const SBPos &pos, SBPosType dist = SBPosType(1))
  {
    return{ pos.x(), pos.y() - dist, pos.z() };
  }

  inline SBPos Side(const SBPos &pos, SideIndex side, SBPosType dist = SBPosType(1))
  {
    using func_ptr = SBPos (*)(const SBPos &, SBPosType);
    static func_ptr funcs[6];
    funcs[ISOUTH] = &South;
    funcs[IEAST] = &East;
    funcs[INORTH] = &North;
    funcs[IWEST] = &West;
    funcs[ITOP] = &Top;
    funcs[IDOWN] = &Down;

    return funcs[side](pos, dist);
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