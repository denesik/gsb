#include "UpdatableArea.h"
#include "UpdatableSectors.h"
#include "World.h"
#include <functional>

void UpdatableArea::OnNewSector(SPos s)
{
  mWorld.GetUpdatableSectors().Add(s);
}

void UpdatableArea::OnDeleteSector(SPos s)
{
  mWorld.GetUpdatableSectors().Remove(s);
}

UpdatableArea::UpdatableArea(World &world, const SPos &pos, int radius)
  : mWorld(world), mPos(pos)
{
  SetRadius(radius);
  SetPos(mPos);

  for (const auto &site : mPositions)
  {
    mWorld.GetUpdatableSectors().Add(site.pos);
  }

  addCon = mPositions.onAdding.connect(std::bind(&UpdatableArea::OnNewSector, this, std::placeholders::_1));
  delCon = mPositions.onDeletting.connect(std::bind(&UpdatableArea::OnDeleteSector, this, std::placeholders::_1));
}

UpdatableArea::~UpdatableArea()
{
  for (const auto &site : mPositions)
  {
    mWorld.GetUpdatableSectors().Remove(site.pos);
  }

  mPositions.onAdding.disconnect(addCon);
  mPositions.onDeletting.disconnect(delCon);
}

void UpdatableArea::SetPos(const SPos &pos)
{
  mPositions.UpdatePos(pos);
}

void UpdatableArea::SetRadius(int radius)
{
  mPositions.SetSize({ radius, radius });
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
