#include "UpdatableArea.h"
#include "UpdatableSectors.h"



UpdatableArea::UpdatableArea(UpdatableSectors &updatable, const SPos &pos, unsigned int radius)
  : mUpdatable(updatable), mPos(pos)
{
  UpdateRadius(radius);
  UpdatePos(mPos);

  for (const auto &site : mPositions)
  {
    mUpdatable.Add(std::get<1>(site));
  }
}


UpdatableArea::~UpdatableArea()
{
  for (const auto &site : mPositions)
  {
    mUpdatable.Remove(std::get<1>(site));
  }
}

void UpdatableArea::SetPos(const SPos &pos)
{
  if (mPos != pos)
  {
    mPos = pos;

    for (const auto &site : mPositions)
    {
      mUpdatable.Remove(std::get<1>(site));
    }

    UpdatePos(mPos);

    for (const auto &site : mPositions)
    {
      mUpdatable.Add(std::get<1>(site));
    }
  }
}

void UpdatableArea::SetRadius(unsigned int radius)
{
  for (const auto &site : mPositions)
  {
    mUpdatable.Remove(std::get<1>(site));
  }

  UpdateRadius(radius);
  UpdatePos(mPos);

  for (const auto &site : mPositions)
  {
    mUpdatable.Add(std::get<1>(site));
  }
}

void UpdatableArea::UpdateRadius(unsigned int radius)
{
  mPositions.clear();

  int begin = -static_cast<int>(radius);
  int end = static_cast<int>(radius);
  SPos pos(begin);
  pos.y() = 0;
  for (pos.z() = begin; pos.z() <= end; ++pos.z())
    for (pos.y() = 0; pos.y() < SECTOR_COUNT_HEIGHT; ++pos.y())
      for (pos.x() = begin; pos.x() <= end; ++pos.x())
      {
        mPositions.emplace_back(pos, SPos{});
      }
}

void UpdatableArea::UpdatePos(const SPos &pos)
{
  for (auto &site : mPositions)
  {
    std::get<1>(site) = std::get<0>(site) + pos;
  }
}

