#pragma once
#include <vector>
#include <numeric>
#include <Magnum\Magnum.h>
#include <tools\CoordSystem.h>

using namespace Magnum;

template<typename T>
class RingBuffer2D
{
public:
  using StoredType = T;

  template<typename ValueType>
  class OwnIterator : public std::iterator<std::input_iterator_tag, ValueType>
  {
    friend class RingBuffer2D;

  private:
    OwnIterator(ValueType* p) : p(p)
    {
    }

  public:
    OwnIterator(const OwnIterator &it) : p(it.p)
    {
    }

    bool operator!=(OwnIterator const& other) const
    {
      return p != other.p;
    }

    bool operator==(OwnIterator const& other) const
    {
      return p == other.p;
    }

    typename iterator::reference operator*() const
    {
      return *p;
    }

    iterator& operator++()
    {
      ++p;
      return *this;
    }

  private:
    ValueType* p;
  };

  using iterator = OwnIterator<T>;
  using const_iterator = OwnIterator<const T>;

  RingBuffer2D<T> &SetSize(Vector2i size)
  {
    mSize = size;
    mStorage.clear();
    mStorage.resize(GetElementsCount());

    for (auto &i : mStorage)
      i.init();

    return *this;
  }

  void UpdatePos(const SPos &pos)
  {
    size_t deltaX = std::max(std::min(mPos.x() - pos.x(), mSize.x()), -mSize.x());
    size_t deltaY = std::max(std::min(mPos.z() - pos.z(), mSize.y()), -mSize.y());

    for (size_t i = std::min(size_t(0), deltaX); i < std::max(size_t(0), deltaX); i++)
      for (size_t j = std::min(size_t(0), deltaY); j < std::max(size_t(0), deltaX); j++)
      {
        SPos spos = pos + SPos(i, 0, j);
        auto wpos = Wrapped(Vector2i(spos.x(),spos.z()));
        mStorage[wpos.x() * mSize.y() + wpos.y()].reset(spos);
      }

    mPos = pos;
  }

  iterator begin()
  {
    return iterator(&mStorage[0]);
  }

  iterator end()
  {
    return iterator(&mStorage[0] + mStorage.size());
  }

private:
  size_t GetElementsCount()
  {
    return (mSize.x() * (mSize.x() - 1)) * (mSize.y() * (mSize.y() - 1));
  }

  size_t WrappedX(size_t x)
  {
    return x % mSize.x();
  }

  size_t WrappedY(size_t y)
  {
    return y % mSize.y();
  }

  Vector2i Wrapped(Vector2i pos)
  {
    return{ pos.x() % mSize.x(), pos.y() % mSize.y() };
  }

  std::vector<T> mStorage;
  Vector2i mSize;
  SPos mPos = {-9999, 0,-99999};
};
