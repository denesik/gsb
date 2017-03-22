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
  SECTOR_SIZE = 16,
  SECTOR_HEIGHT = 256,
  SECTOR_CAPACITY = SECTOR_SIZE * SECTOR_SIZE * SECTOR_HEIGHT,

  TESSELATOR_SIZE = SECTOR_SIZE + 2,
  TESSELATOR_HEIGHT = SECTOR_HEIGHT + 2,
  TESSELATOR_CAPACITY = TESSELATOR_SIZE * TESSELATOR_SIZE * TESSELATOR_HEIGHT,
};

// TODO BOTTOM --> DOWN
enum SideIndex : int
{
  IFRONT = 0,
  IRIGHT = 1,
  IBACK = 2,
  ILEFT = 3,
  ITOP = 4,
  IBOTTOM = 5,
};

enum SideFlags : int
{
  NONE = 0,

  FRONT = 1 << IFRONT,
  RIGHT = 1 << IRIGHT,
  BACK = 1 << IBACK,
  LEFT = 1 << ILEFT,
  TOP = 1 << ITOP,
  BOTTOM = 1 << IBOTTOM,

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


static const SPos SectorSize = { SECTOR_SIZE , SECTOR_HEIGHT , SECTOR_SIZE };
static const SPos TesselatorSize = { TESSELATOR_SIZE , TESSELATOR_HEIGHT , TESSELATOR_SIZE };

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

    spos.x() = (pos.x() >= 0) ? static_cast<SPosType>(pos.x()) / SectorSize.x() : (static_cast<SPosType>(pos.x()) - SectorSize.x() + SPosType(1)) / SectorSize.x();
    spos.y() = (pos.y() >= 0) ? static_cast<SPosType>(pos.y()) / SectorSize.y() : (static_cast<SPosType>(pos.y()) - SectorSize.y() + SPosType(1)) / SectorSize.y();
    spos.z() = (pos.z() >= 0) ? static_cast<SPosType>(pos.z()) / SectorSize.z() : (static_cast<SPosType>(pos.z()) - SectorSize.z() + SPosType(1)) / SectorSize.z();

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
    return wbpos - spos * SectorSize;
  }

  /// ћировые координаты в координаты блока в секторе.
  inline SBPos WtoSB(const WPos &pos, const SPos &spos)
  {
    return WtoWB(pos) - spos * SectorSize;
  }

  ///  оординаты блока в мире в координаты блока в секторе.
  inline SBPos WBtoSB(const WBPos &pos)
  {
    auto spos = WBtoS(pos);
    return pos - spos * SectorSize;
  }

  ///  оординаты блока в мире в координаты блока в секторе.
  inline SBPos WBtoSB(const WBPos &pos, const SPos &spos)
  {
    return pos - spos * SectorSize;
  }

  ///  оординаты блока в секторе в координаты блока в мире.
  inline WBPos SBtoWB(const SBPos &pos, const SPos &spos)
  {
    return spos * SectorSize + pos;
  }

  ///  оординаты сектора в мировые координаты.
  /// ¬озвращаютс€ координаты нулевого блока в секторе.
  inline WPos StoW(const SPos &pos)
  {
    WPos wpos(pos * SectorSize);
    return wpos;
  }

  /// »ндекс блока в секторе в позицию блока в секторе.
  inline SBPos BItoSB(IndexType i)
  {
    return SBPos{ 
      static_cast<SBPosType>(i % SectorSize.x()),
      static_cast<SBPosType>((i / SectorSize.x()) % SectorSize.y()),
      static_cast<SBPosType>(i / (SectorSize.x() * SectorSize.y())) };
  }

  /// ѕозици€ блока в секторе в индекс блока в секторе.
  inline IndexType SBtoBI(const SBPos &pos)
  {
    return static_cast<IndexType>(pos.z()) * SectorSize.x() * SectorSize.y() +
           static_cast<IndexType>(pos.y()) * SectorSize.x() +
           static_cast<IndexType>(pos.x());
  }

  /// »ндекс тессел€тора в секторе в позицию тессел€тора в секторе.
  inline STPos TItoST(IndexType i)
  {
    return SBPos{ 
      static_cast<SBPosType>(i % SectorSize.x()),
      static_cast<SBPosType>((i / SectorSize.x()) % SectorSize.y()),
      static_cast<SBPosType>(i / (SectorSize.x() * SectorSize.y())) };
  }

  /// ѕозици€ тессел€тора в секторе в индекс тессел€тора в секторе.
  inline IndexType STtoTI(const STPos &pos)
  {
    return static_cast<IndexType>(pos.z()) * TesselatorSize.x() * TesselatorSize.y() +
      static_cast<IndexType>(pos.y()) * TesselatorSize.x() +
      static_cast<IndexType>(pos.x());
  }

  /// »ндекс тессел€тора в секторе в позицию блока в секторе.
  inline SBPos TItoSB(IndexType i)
  {
    assert(i % TesselatorSize.x() > 0 &&
      (i / TesselatorSize.x()) % TesselatorSize.y() > 0 &&
      i / (TesselatorSize.x() * TesselatorSize.y()) > 0);
    return SBPos{
      static_cast<SBPosType>(i % TesselatorSize.x() - 1),
      static_cast<SBPosType>((i / TesselatorSize.x()) % TesselatorSize.y() - 1),
      static_cast<SBPosType>(i / (TesselatorSize.x() * TesselatorSize.y()) - 1) };
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

  inline SBPos Side(const SBPos &pos, SideIndex side)
  {
    using func_ptr = SBPos (*)(const SBPos &);
    static func_ptr funcs[6];
    funcs[IFRONT] = &Front;
    funcs[IRIGHT] = &Right;
    funcs[IBACK] = &Back;
    funcs[ILEFT] = &Left;
    funcs[ITOP] = &Top;
    funcs[IBOTTOM] = &Bottom;

    return funcs[side](pos);
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