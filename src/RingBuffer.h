#pragma once
#include <vector>
#include <numeric>
#include <Magnum\Magnum.h>
#include <tools\CoordSystem.h>
#include <functional>

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
    for (int i = -size.x(); i <= size.x(); i++)
      for (int j = -size.y(); j <= size.y(); j++)
      {
        SPos spos = mPos + SPos(i, 0, j);
        if (onDeletting) onDeletting(spos);
      }

    mRadius = size;
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

        if (onAdding) onAdding(spos);
      }

    return *this;
  }

  boost::optional<T &> Get(Magnum::Vector2i spos)
  {
    if ((std::abs(mPos.x() - spos.x()) < mRadius.x()) || (std::abs(mPos.y() - spos.y()) < mRadius.y()))
      return{};

    Magnum::Vector2i sec_pos = WrapSPos({ spos.x(), spos.z() });
    Magnum::Vector2i wpos = spos - sec_pos * mDim;

    return mStorage[wpos.y() * mDim.x() + wpos.x()];
  }

  template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
  }

  //TODO: тестить на больших дельтах
  void UpdatePos(const SPos &pos)
  {
    if (pos == mPos)
      return;

    int deltaX = std::max(std::min(pos.x() - mPos.x(), mRadius.x() * 2), -mRadius.x() * 2);
    int deltaY = std::max(std::min(pos.z() - mPos.z(), mRadius.y() * 2), -mRadius.y() * 2);

    //новые столбцы
    for (int i = 0; i < std::abs(deltaX); i++)
      for (int j = 0; j < mDim.y(); j++)
      {
        // новые столбцы будут в координатах центр(pos) + радиус(mSize.x()) + номер столбца(i)
        // для отрицательных координат центр(pos) - радиус(mSize.x()) - номер столбца(i)\
        // потому в таком случае они умножаются на -1(sgn(deltaX))
        SPos spos = pos + SPos((mRadius.x() + i) * sgn(deltaX), 0, j - mRadius.y());

        // координаты очередного элемента в виртуальных секторах, размером radius*2+1
        Magnum::Vector2i sec_pos = WrapSPos({ spos.x(), spos.z() });

        // приведение координат к [0, radius*2+1)
        Magnum::Vector2i wpos = Magnum::Vector2i(spos.x(), spos.z()) - mDim * sec_pos;
        mStorage[wpos.y() * mDim.x() + wpos.x()].reset(spos);

        if (onAdding) onAdding(spos);
        if (onDeletting) onDeletting(spos - SPos(mDim.x(), 0, 0));
      }

    //новые строки
    for (int j = 0; j < std::abs(deltaY); j++)
      for (int i = 0; i < mDim.x(); i++)
      {
        // все аналогично, читай выше
        SPos spos = pos + SPos(i - mRadius.x(), 0, (mRadius.y() + j) * sgn(deltaY));
        Magnum::Vector2i sec_pos = WrapSPos({ spos.x(), spos.z() });
        Magnum::Vector2i wpos = Magnum::Vector2i(spos.x(), spos.z()) - mDim * sec_pos;
        mStorage[wpos.y() * mDim.x() + wpos.x()].reset(spos);

        if (onAdding) onAdding(spos);
        if (onDeletting) onDeletting(spos - SPos(0, 0, mDim.y()));
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

  std::function<void(const SPos &)> onAdding;
  std::function<void(const SPos &)> onDeletting;

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
