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
};

typedef Magnum::Vector3   WPos;     // ������� ����������.
typedef Magnum::Vector3i  SPos;     // ���������� �������.
typedef Magnum::Vector3i  WBPos;    // ���������� ����� � ����.
typedef Magnum::Vector3i  SBPos;    // ���������� ����� � �������.
typedef Magnum::Vector3i  STPos;    // ���������� ����������� � �������.
typedef Magnum::Float WPosType;
typedef Magnum::Int SPosType;
typedef Magnum::Int WBPosType;
typedef Magnum::Int SBPosType;
typedef Magnum::Int STPosType;
typedef Magnum::UnsignedInt IndexType;

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
    const SPosType size = static_cast<SPosType>(SECTOR_SIZE);

    spos.x() = (pos.x() >= 0) ? static_cast<SPosType>(pos.x()) / size : (static_cast<SPosType>(pos.x()) - size + SPosType(1)) / size;
    spos.y() = (pos.y() >= 0) ? static_cast<SPosType>(pos.y()) / size : (static_cast<SPosType>(pos.y()) - size + SPosType(1)) / size;
    spos.z() = (pos.z() >= 0) ? static_cast<SPosType>(pos.z()) / size : (static_cast<SPosType>(pos.z()) - size + SPosType(1)) / size;

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
    const SPosType size = static_cast<SPosType>(SECTOR_SIZE);

    return wbpos - spos * size;
  }

  /// ������� ���������� � ���������� ����� � �������.
  inline SBPos WtoSB(const WPos &pos, const SPos &spos)
  {
    const SBPosType size = static_cast<SBPosType>(SECTOR_SIZE);

    return WtoWB(pos) - spos * size;
  }

  /// ���������� ����� � ���� � ���������� ����� � �������.
  inline SBPos WBtoSB(const WBPos &pos)
  {
    auto spos = WBtoS(pos);
    const SPosType size = static_cast<SPosType>(SECTOR_SIZE);

    return pos - spos * size;
  }

  /// ���������� ����� � ���� � ���������� ����� � �������.
  inline SBPos WBtoSB(const WBPos &pos, const SPos &spos)
  {
    const SBPosType size = static_cast<SBPosType>(SECTOR_SIZE);

    return pos - spos * size;
  }

  /// ���������� ����� � ������� � ���������� ����� � ����.
  inline WBPos SBtoWB(const SBPos &pos, const SPos &spos)
  {
    const WBPosType size = static_cast<WBPosType>(SECTOR_SIZE);

    return spos * size + pos;
  }

  /// ���������� ������� � ������� ����������.
  /// ������������ ���������� �������� ����� � �������.
  inline WPos StoW(const SPos &pos)
  {
    WPos wpos(pos);
    const WPosType size = static_cast<WPosType>(SECTOR_SIZE);

    return wpos * size;
  }

  /// ������ ����� � ������� � ������� ����� � �������.
  inline SBPos BItoSB(IndexType i)
  {
    const IndexType size = static_cast<IndexType>(SECTOR_SIZE);
    return SBPos{ 
      static_cast<SBPosType>(i % size), 
      static_cast<SBPosType>((i / size) % size), 
      static_cast<SBPosType>(i / (size * size)) };
  }

  /// ������� ����� � ������� � ������ ����� � �������.
  inline IndexType SBtoBI(const SBPos &pos)
  {
    const IndexType size = static_cast<IndexType>(SECTOR_SIZE);
    return static_cast<IndexType>(pos.z()) * size * size +
           static_cast<IndexType>(pos.y()) * size +
           static_cast<IndexType>(pos.x());
  }

  /// ������ ����������� � ������� � ������� ����������� � �������.
  inline STPos TItoST(IndexType i)
  {
    const IndexType size = static_cast<IndexType>(SECTOR_SIZE);
    return STPos{
      static_cast<STPosType>(i % size),
      static_cast<STPosType>((i / size) % size),
      static_cast<STPosType>(i / (size * size)) };
  }

  /// ������� ����������� � ������� � ������ ����������� � �������.
  inline IndexType STtoTI(const STPos &pos)
  {
    const IndexType size = static_cast<IndexType>(TESSELATOR_SIZE);
    return static_cast<IndexType>(pos.z()) * size * size +
           static_cast<IndexType>(pos.y()) * size +
           static_cast<IndexType>(pos.x());
  }

  /// ������ ����������� � ������� � ������� ����� � �������.
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