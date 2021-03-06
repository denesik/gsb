#pragma once
#include <vector>
#include <numeric>
#include <Magnum\Magnum.h>
#include <tools\CoordSystem.h>
#include <functional>
#include <boost\optional.hpp>

#define GSB_TEST_BUFFER
#ifndef GSB_TEST_BUFFER

namespace
{
  inline void StoreOrderedInc(Magnum::Vector2i & pos, Magnum::Vector2i radius)
  {
    pos.x()++;
    if (pos.x() > radius.x())
    {
      pos.x() = -radius.x();
      pos.y()++;
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
      Magnum::Vector2i unoffseted = -mRadius;
      for (int i = 0; i < mDim.x() * mDim.y(); i++)
      {
        Magnum::Vector2i spos = ((ItoPos(i) - mOffset) % mRadius) + mCenter;
        
        onDeletting(mStorage[i], SPos(spos.x(), 0, spos.y()));

        StoreOrderedInc(unoffseted, mRadius);
      }
    }

    mRadius = size;
    mDim = size * 2 + Magnum::Vector2i(1, 1);
    mStorage.clear();

    Magnum::Vector2i unoffseted = -mRadius;
    for (int i = 0; i < mDim.x() * mDim.y(); i++)
    {
      Magnum::Vector2i spos = mCenter + unoffseted;

      mStorage.emplace_back(onAdding(SPos(spos.x(), 0, spos.y())));

      StoreOrderedInc(unoffseted, mRadius);
    }

    mOffset = {};

    return *this;
  }

  boost::optional<T &> Get(const SPos & _spos)
  {
    auto spos = Magnum::Vector2i(_spos.x(), _spos.z());
    if ((std::abs(mCenter.x() - spos.x()) > mRadius.x()) || (std::abs(mCenter.y() - spos.y()) > mRadius.y()))
      return{};

    auto locPos = spos - mCenter + mOffset;
    auto wraped = WrapSPos(locPos);
    auto t = PosToIndex(wraped);
    return mStorage[t];
  }

  template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
  }

  //TODO: ������� �� ������� �������
  void UpdatePos(const SPos & _newCenter)
  {
    auto newCenter = Magnum::Vector2i(_newCenter.x(), _newCenter.z());

    if (newCenter == mCenter)
      return;

    mOffset -= mCenter - newCenter;

    Magnum::Vector2i unoffseted = -mRadius;
    for (int i = 0; i < mDim.x() * mDim.y(); i++)
    {
      Magnum::Vector2i oldPos = mCenter + unoffseted;
      if (std::abs(oldPos.x() - newCenter.x()) > mRadius.x() || std::abs(oldPos.y() - newCenter.y()) > mRadius.y())
      {
        auto wraped = WrapSPos(oldPos);
        auto t = PosToIndex(wraped);
        onDeletting(mStorage[t], SPos(oldPos.x(), 0, oldPos.y()));
      }

      StoreOrderedInc(unoffseted, mRadius);
    }

    unoffseted = -mRadius;
    for (int i = 0; i < mDim.x() * mDim.y(); i++)
    {
      Magnum::Vector2i newPos = newCenter + unoffseted;
      if (std::abs(newPos.x() - mCenter.x()) > mRadius.x() || std::abs(newPos.y() - mCenter.y()) > mRadius.y())
      {
        auto wraped = WrapSPos(newPos);
        auto t = PosToIndex(wraped);
        mStorage[t] = onAdding(SPos(newPos.x(), 0, newPos.y()));
      }

      StoreOrderedInc(unoffseted, mRadius);
    }

    mCenter = newCenter;
  }

  const SPos &GetPos()
  {
    return{ mCenter.x(), 0, mCenter.y() };
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
  Magnum::Vector2i WrapSPos(const Magnum::Vector2i &val)
  {
    Magnum::Vector2i spos = val;

    if (val.x() >  mRadius.x()) spos.x() = ((val.x() + mRadius.x()) % mDim.x()) - mRadius.x();
    if (val.x() < -mRadius.x()) spos.x() = ((val.x() - mRadius.x()) % mDim.x()) + mRadius.x();

    if (val.y() >  mRadius.x()) spos.y() = ((val.y() + mRadius.y()) % mDim.y()) - mRadius.y();
    if (val.y() < -mRadius.x()) spos.y() = ((val.y() - mRadius.y()) % mDim.y()) + mRadius.y();

    return spos;
  }

  Magnum::Vector2i ItoPos(int i)
  {
    return { i % mDim.x() - mRadius.x(), (i / mDim.x()) % mDim.y() - mRadius.y() };
  }

  int PosToIndex(const Magnum::Vector2i &val)
  {
    auto valOff = val + mRadius;
    return valOff.y() * mDim.x() + valOff.x();
  }

  std::vector<T> mStorage;
  Magnum::Vector2i mRadius;
  Magnum::Vector2i mDim;
  Magnum::Vector2i mCenter;
  Magnum::Vector2i mOffset;
};

#else

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
  }

  RingBuffer<T> &SetSize(Magnum::Vector2i size)
  {
    if (size == mSize)
      return *this;
    mSize = size;
    Update();
    return *this;
  }

  boost::optional<T &> Get(const SPos &spos)
  {
    auto it = std::find_if(mPositions.begin(), mPositions.end(), [&spos](const SPos &pos)
    {
      return pos == spos;
    });

    if (it != mPositions.end())
    {
      return mStorage[std::distance(mPositions.begin(), it)];
    }

    return{};
  }

  void UpdatePos(const SPos &pos)
  {
    if (pos == mPos)
      return;
    mPos = pos;
    Update();
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
  std::vector<T> mStorage;
  std::vector<SPos> mPositions;

  Magnum::Vector2i mSize;
  SPos mPos;

private:
  std::vector<SPos> Generate(const SPos &pos, const Magnum::Vector2i &size)
  {
    std::vector<SPos> data;
    for (int y = -size.y(); y <= size.y(); ++y)
      for (int x = -size.x(); x <= size.x(); ++x)
      {
        data.emplace_back(pos + SPos{ x, 0, y });
      }
    return data;
  }

  std::vector<SPos> Association(const std::vector<SPos> &a, const std::vector<SPos> &b)
  {
    std::vector<SPos> data;
    for (const auto &pos : a)
    {
      auto it = std::find_if(b.begin(), b.end(), [&pos](const SPos &val)
      {
        return val == pos;
      });

      if (it != b.end())
      {
        data.emplace_back(pos);
      }
    }
    return data;
  }

  std::vector<SPos> Subtraction(const std::vector<SPos> &a, const std::vector<SPos> &b)
  {
    std::vector<SPos> data;
    for (const auto &pos : a)
    {
      auto it = std::find_if(b.begin(), b.end(), [&pos](const SPos &val)
      {
        return val == pos;
      });

      if (it == b.end())
      {
        data.emplace_back(pos);
      }
    }
    return data;
  }

  void Update()
  {
    auto new_data = Generate(mPos, mSize);
    auto &old_data = mPositions;
    auto delta = Association(old_data, new_data);
    auto old_delta = Subtraction(old_data, delta);
    auto new_delta = Subtraction(new_data, delta);

    for (const auto &pos : old_delta)
    {
      auto it = std::find_if(mPositions.begin(), mPositions.end(), [&pos](const SPos &spos)
      {
        return pos == spos;
      });
      auto index = std::distance(mPositions.begin(), it);
      onDeletting(mStorage[index], mPositions[index]);
      std::swap(mStorage[index], mStorage.back());
      mStorage.pop_back();
      std::swap(mPositions[index], mPositions.back());
      mPositions.pop_back();
    }

    for (const auto &pos : new_delta)
    {
      //       auto it = std::find_if(mPositions.begin(), mPositions.end(), [&pos](const SPos &spos)
      //       {
      //         return pos == spos;
      //       });
      //       auto index = std::distance(mPositions.begin(), it);
      mPositions.emplace_back(pos);
      mStorage.emplace_back(onAdding(pos));
    }
  }

};

#undef GSB_TEST_BUFFER
#endif


