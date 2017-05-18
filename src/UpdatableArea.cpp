#include "UpdatableArea.h"
#include "UpdatableSectors.h"
#include "World.h"



UpdatableArea::UpdatableArea(World &world, const SPos &pos, unsigned int radius)
  : mWorld(world), mPos(pos)
{
  UpdateRadius(radius);
  UpdatePos(mPos);

  for (const auto &site : mPositions)
  {
    mWorld.GetUpdatableSectors().Add(std::get<1>(site));
  }
}


UpdatableArea::~UpdatableArea()
{
  for (const auto &site : mPositions)
  {
    mWorld.GetUpdatableSectors().Remove(std::get<1>(site));
  }
}

void UpdatableArea::SetPos(const SPos &pos)
{
  if (mPos != pos)
  {
    mPos = pos;

    for (const auto &site : mPositions)
    {
      mWorld.GetUpdatableSectors().Remove(std::get<1>(site));
    }

    UpdatePos(mPos);

    for (const auto &site : mPositions)
    {
      mWorld.GetUpdatableSectors().Add(std::get<1>(site));
    }
  }
}

void UpdatableArea::SetRadius(unsigned int radius)
{
  for (const auto &site : mPositions)
  {
    mWorld.GetUpdatableSectors().Remove(std::get<1>(site));
  }

  UpdateRadius(radius);
  UpdatePos(mPos);

  for (const auto &site : mPositions)
  {
    mWorld.GetUpdatableSectors().Add(std::get<1>(site));
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

SectorLoader::SectorLoader(World &world)
  : mWorld(world),
  mBufferData(
    [&world](const SPos &pos)
    {
      world.LoadSector(pos);
      //LOG(trace) << "Load sector [" << pos.x() << " " << pos.z() << "]. Count: ";
      return 0;
    },
    [&world](int &data, const SPos &pos)
    {
      world.UnLoadSector(pos);
      //LOG(trace) << "UnLoad sector [" << pos.x() << " " << pos.z() << "]. Count: ";
    })
{

}

SectorLoader::~SectorLoader()
{

}

void SectorLoader::SetPos(const SPos &pos)
{
  mBufferData.UpdatePos(pos);
}

void SectorLoader::SetRadius(unsigned int radius)
{
  mBufferData.SetSize({ static_cast<int>(radius), static_cast<int>(radius) });
}
