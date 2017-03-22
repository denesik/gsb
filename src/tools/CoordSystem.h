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
  IFRONT = 0,
  IRIGHT = 1,
  IBACK = 2,
  ILEFT = 3,
  ITOP = 4,
  IDOWN = 5,
};

enum SideFlags : int
{
  NONE = 0,

  FRONT = 1 << IFRONT,
  RIGHT = 1 << IRIGHT,
  BACK = 1 << IBACK,
  LEFT = 1 << ILEFT,
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

using CSPos     = Magnum::Vector2i; /// ���������� ������� ��������
using WPos      = Magnum::Vector3;  /// ������� ����������.
using SPos      = Magnum::Vector3i; /// ���������� �������.
using WBPos     = Magnum::Vector3i; /// ���������� ����� � ����.
using SBPos     = Magnum::Vector3i; /// ���������� ����� � �������.
using STPos     = Magnum::Vector3i; /// ���������� ����������� � �������.
using WPosType  = Magnum::Float;
using SPosType  = Magnum::Int;
using WBPosType = Magnum::Int;
using SBPosType = Magnum::Int;
using STPosType = Magnum::Int;
using IndexType = Magnum::UnsignedInt;


constexpr SPos gSectorSize(16, 256, 16);
constexpr SPos gTesselatorSize(gSectorSize.x() + 2, gSectorSize.y() + 2, gSectorSize.z() + 2);
constexpr Magnum::Int gSectorCapacity = gSectorSize.x() * gSectorSize.y() * gSectorSize.z();
constexpr Magnum::Int gTesselatorCapacity = gTesselatorSize.x() * gTesselatorSize.y() * gTesselatorSize.z();

/// ������� ���������.
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

  /// ������� ���������� � ���������� ����� � ����.
  inline WBPos WtoWB(const WPos &pos)
  {
    WBPos wbpos;

    wbpos.x() = (pos.x() >= 0) ? static_cast<WBPosType>(pos.x()) : static_cast<WBPosType>(detail::move<WBPosType>(pos.x())) - WBPosType(1);
    wbpos.y() = (pos.y() >= 0) ? static_cast<WBPosType>(pos.y()) : static_cast<WBPosType>(detail::move<WBPosType>(pos.y())) - WBPosType(1);
    wbpos.z() = (pos.z() >= 0) ? static_cast<WBPosType>(pos.z()) : static_cast<WBPosType>(detail::move<WBPosType>(pos.z())) - WBPosType(1);

    return wbpos;
  }

  /// ���������� ����� � ���� � ���������� �������.
  inline SPos WBtoS(const WBPos &pos)
  {
    SPos spos;

    spos.x() = (pos.x() >= 0) ? static_cast<SPosType>(pos.x()) / gSectorSize.x() : (static_cast<SPosType>(pos.x()) - gSectorSize.x() + SPosType(1)) / gSectorSize.x();
    spos.y() = (pos.y() >= 0) ? static_cast<SPosType>(pos.y()) / gSectorSize.y() : (static_cast<SPosType>(pos.y()) - gSectorSize.y() + SPosType(1)) / gSectorSize.y();
    spos.z() = (pos.z() >= 0) ? static_cast<SPosType>(pos.z()) / gSectorSize.z() : (static_cast<SPosType>(pos.z()) - gSectorSize.z() + SPosType(1)) / gSectorSize.z();

    return spos;
  }

  /// ������� ���������� � ���������� �������.
  inline SPos WtoS(const WPos &pos)
  {
    return WBtoS(WtoWB(pos));
  }

  /// ������� ���������� � ���������� ����� � �������.
  inline SBPos WtoSB(const WPos &pos)
  {
    auto wbpos = WtoWB(pos);
    auto spos = WBtoS(wbpos);
    return wbpos - spos * gSectorSize;
  }

  /// ������� ���������� � ���������� ����� � �������.
  inline SBPos WtoSB(const WPos &pos, const SPos &spos)
  {
    return WtoWB(pos) - spos * gSectorSize;
  }

  /// ���������� ����� � ���� � ���������� ����� � �������.
  inline SBPos WBtoSB(const WBPos &pos)
  {
    auto spos = WBtoS(pos);
    return pos - spos * gSectorSize;
  }

  /// ���������� ����� � ���� � ���������� ����� � �������.
  inline SBPos WBtoSB(const WBPos &pos, const SPos &spos)
  {
    return pos - spos * gSectorSize;
  }

  /// ���������� ����� � ������� � ���������� ����� � ����.
  inline WBPos SBtoWB(const SBPos &pos, const SPos &spos)
  {
    return spos * gSectorSize + pos;
  }

  /// ���������� ������� � ������� ����������.
  /// ������������ ���������� �������� ����� � �������.
  inline WPos StoW(const SPos &pos)
  {
    WPos wpos(pos * gSectorSize);
    return wpos;
  }

  /// ������ ����� � ������� � ������� ����� � �������.
  inline SBPos BItoSB(IndexType i)
  {
    return SBPos{ 
      static_cast<SBPosType>(i % gSectorSize.x()),
      static_cast<SBPosType>((i / gSectorSize.x()) % gSectorSize.y()),
      static_cast<SBPosType>(i / (gSectorSize.x() * gSectorSize.y())) };
  }

  /// ������� ����� � ������� � ������ ����� � �������.
  inline IndexType SBtoBI(const SBPos &pos)
  {
    return static_cast<IndexType>(pos.z()) * gSectorSize.x() * gSectorSize.y() +
           static_cast<IndexType>(pos.y()) * gSectorSize.x() +
           static_cast<IndexType>(pos.x());
  }

  /// ������ ����������� � ������� � ������� ����������� � �������.
  inline STPos TItoST(IndexType i)
  {
    return SBPos{ 
      static_cast<SBPosType>(i % gSectorSize.x()),
      static_cast<SBPosType>((i / gSectorSize.x()) % gSectorSize.y()),
      static_cast<SBPosType>(i / (gSectorSize.x() * gSectorSize.y())) };
  }

  /// ������� ����������� � ������� � ������ ����������� � �������.
  inline IndexType STtoTI(const STPos &pos)
  {
    return static_cast<IndexType>(pos.z()) * gTesselatorSize.x() * gTesselatorSize.y() +
      static_cast<IndexType>(pos.y()) * gTesselatorSize.x() +
      static_cast<IndexType>(pos.x());
  }

  /// ������ ����������� � ������� � ������� ����� � �������.
  inline SBPos TItoSB(IndexType i)
  {
    assert(i % gTesselatorSize.x() > 0 &&
      (i / gTesselatorSize.x()) % gTesselatorSize.y() > 0 &&
      i / (gTesselatorSize.x() * gTesselatorSize.y()) > 0);
    return SBPos{
      static_cast<SBPosType>(i % gTesselatorSize.x() - 1),
      static_cast<SBPosType>((i / gTesselatorSize.x()) % gTesselatorSize.y() - 1),
      static_cast<SBPosType>(i / (gTesselatorSize.x() * gTesselatorSize.y()) - 1) };
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

  inline SBPos Down(const SBPos &pos)
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
    funcs[IDOWN] = &Down;

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