#pragma once
#include <vector>
#include <numeric>
#include <Magnum\Magnum.h>
#include <tools\CoordSystem.h>
#include <functional>
#include <boost\optional.hpp>

namespace
{
  inline void StoreOrderedInc(SPos & pos, Magnum::Vector2i radius)
  {
    pos.z()++;
    if (pos.z() > radius.y())
    {
      pos.z() = -radius.y();
      pos.x()++;
    }
  }
}

template<typename T>
class RingBuffer
{
public:
  using StoredType = T;
  using AddFunction = std::function<T(const SPos &)>;
  using DeleteFunction = std::function<void(T &, const SPos &)>;

  RingBuffer<T>(AddFunction add, DeleteFunction del)
    : onAdding(add)
    , onDeletting(del)
  {
    //SetSize(size);
  }

  RingBuffer<T> &SetSize(Magnum::Vector2i size)
  {
    if (!mStorage.empty())
    {
      SPos unoffseted = { -mRadius.x(), 0, -mRadius.y() };
      for (int i = 0; i < mDim.x() * mDim.y(); i++)
      {
        SPos spos = mPos + unoffseted;
        onDeletting(mStorage[i], spos);

        StoreOrderedInc(unoffseted, mRadius);
      }
    }

    mRadius = size;
    mDim = size * 2 + Magnum::Vector2i(1, 1);
    mStorage.clear();

    SPos unoffseted = { -mRadius.x(), 0, -mRadius.y() };
    for (int i = 0; i < mDim.x() * mDim.y(); i++)
    {
      SPos spos = mPos + unoffseted;

      mStorage.emplace_back(onAdding(spos));

      StoreOrderedInc(unoffseted, mRadius);
    }

    return *this;
  }

  boost::optional<T &> Get(const SPos &spos)
  {
    if ((std::abs(mPos.x() - spos.x()) > mRadius.x()) || (std::abs(mPos.z() - spos.z()) > mRadius.y()))
      return{};

    const SPos spos2 = (spos - mPos) + SPos(mRadius.x(), 0, mRadius.y());
    auto t = spos2.x() * mDim.y() + spos2.z();
    return mStorage[t];
  }

  template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
  }

  //TODO: тестить на больших дельтах
  void UpdatePos(const SPos &pos)
  {
    if (pos == mPos)
      return;

    SPos unoffseted = { -mRadius.x(), 0, -mRadius.y() };
    for (int i = 0; i < mDim.x() * mDim.y(); i++)
    {
      SPos oldPos = mPos + unoffseted;
      if (std::abs(oldPos.x() - pos.x()) > mRadius.x() || std::abs(oldPos.z() - pos.z()) > mRadius.y())
      {
        onDeletting(mStorage[i], oldPos);
      }

      SPos newPos = pos + unoffseted;
      if (std::abs(mPos.x() - newPos.x()) > mRadius.x() || std::abs(mPos.z() - newPos.z()) > mRadius.y())
      {
        mStorage[i] = onAdding(newPos);
      }

      StoreOrderedInc(unoffseted, mRadius);
    }

    mPos = pos;
  }

  const SPos &GetPos()
  {
    return mPos;
  }

  auto begin()
  {
    return mStorage.begin();
  }

  auto end()
  {
    return mStorage.end();
  }

  AddFunction onAdding;
  DeleteFunction onDeletting;

private:
  inline Magnum::Vector2i WrapSPos(const Magnum::Vector2i &pos)
  {
    Magnum::Vector2i spos;

    spos.x() = (pos.x() >= 0) ? static_cast<int>(pos.x()) / mDim.x() : (static_cast<int>(pos.x()) - mDim.x() + int(1)) / mDim.x();
    spos.y() = (pos.y() >= 0) ? static_cast<int>(pos.y()) / mDim.y() : (static_cast<int>(pos.y()) - mDim.y() + int(1)) / mDim.y();

    return spos;
  }

  std::vector<T> mStorage;
  Magnum::Vector2i mRadius;
  Magnum::Vector2i mDim;
  SPos mPos;
};