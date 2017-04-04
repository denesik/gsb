#pragma once
#include <vector>
#include <numeric>
#include <Magnum\Magnum.h>
#include <tools\CoordSystem.h>

template<typename T>
class RingBuffer2D
{
public:
  using StoredType = T;

  RingBuffer2D<T>(Magnum::Vector2i size = { 3, 3 })
  {
    SetSize(size);
  }

  RingBuffer2D<T> &SetSize(Magnum::Vector2i size)
  {
    mSize = size;
    mDim = size * 2 + Magnum::Vector2i(1, 1);
    mStorage.clear();
    mStorage.resize(mDim.x() * mDim.y());

    for (auto &i : mStorage)
      i.init();

    for (int i = -size.x(); i <= size.x(); i++)
      for (int j = -size.y(); j <= size.y(); j++)
      {
        SPos spos = mPos + SPos(i, 0, j);
        Magnum::Vector2i sec_pos = WrapSPos({ spos.x(), spos.z() });
        Magnum::Vector2i wpos = Magnum::Vector2i(spos.x(), spos.z()) - mDim * sec_pos;
        mStorage[wpos.y() * mDim.x() + wpos.x()].reset(spos);
      }

    return *this;
  }

  template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
  }

  //TODO: тестить на больших дельтах
  void UpdatePos(const SPos &pos)
  {
    if (pos == mPos)
      return;

    int deltaX = std::max(std::min(pos.x() - mPos.x(), mSize.x() * 2), -mSize.x() * 2);
    int deltaY = std::max(std::min(pos.z() - mPos.z(), mSize.y() * 2), -mSize.y() * 2);

    //новые столбцы
    for (int i = 0; i < std::abs(deltaX); i++)
      for (int j = 0; j < mDim.y(); j++)
      {
        // новые столбцы будут в координатах центр(pos) + радиус(mSize.x()) + номер столбца(i)
        // для отрицательных координат центр(pos) - радиус(mSize.x()) - номер столбца(i)\
        // потому в таком случае они умножаются на -1(sgn(deltaX))
        SPos spos = pos + SPos(mSize.x() * sgn(deltaX) + (i)* sgn(deltaX), 0, j - mSize.y());

        // координаты очередного элемента в виртуальных секторах, размером radius*2+1
        Magnum::Vector2i sec_pos = WrapSPos({ spos.x(), spos.z() });

        // приведение координат к [0, radius*2+1)
        Magnum::Vector2i wpos = Magnum::Vector2i(spos.x(), spos.z()) - mDim * sec_pos;
        mStorage[wpos.y() * mDim.x() + wpos.x()].reset(spos);
      }

    //новые строки
    for (int j = 0; j < std::abs(deltaY); j++)
      for (int i = 0; i < mDim.x(); i++)
      {
        SPos spos = pos + SPos(i - mSize.x(), 0, mSize.y() * sgn(deltaY) + (j)* sgn(deltaY));
        Magnum::Vector2i sec_pos = WrapSPos({ spos.x(), spos.z() });
        Magnum::Vector2i wpos = Magnum::Vector2i(spos.x(), spos.z()) - mDim * sec_pos;
        mStorage[wpos.y() * mDim.x() + wpos.x()].reset(spos);
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

  inline Magnum::Vector2i WrapSPos(const Magnum::Vector2i &pos)
  {
    Magnum::Vector2i spos;

    spos.x() = (pos.x() >= 0) ? static_cast<int>(pos.x()) / mDim.x() : (static_cast<int>(pos.x()) - mDim.x() + int(1)) / mDim.x();
    spos.y() = (pos.y() >= 0) ? static_cast<int>(pos.y()) / mDim.y() : (static_cast<int>(pos.y()) - mDim.y() + int(1)) / mDim.y();

    return spos;
  }

  std::vector<T> mStorage;
  Magnum::Vector2i mSize = { 3, 3 };
  Magnum::Vector2i mDim = { 7, 7 };
  SPos mPos = { 4,0,4 };
};
