#include "WorldSectorObserver.h"



WorldSectorObserver::WorldSectorObserver()
{
}


WorldSectorObserver::~WorldSectorObserver()
{
}

void WorldSectorObserver::Load(Sector &sector)
{
  notify(&IWorldSectorEvent::Load, sector);
}

void WorldSectorObserver::UnLoad(Sector &sector)
{
  notify(&IWorldSectorEvent::UnLoad, sector);
}

IWorldSectorEvent::~IWorldSectorEvent()
{
}
