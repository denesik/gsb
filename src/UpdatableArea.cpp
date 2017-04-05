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

