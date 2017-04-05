#include "WorldSectorObserver.h"



WorldSectorObserver::WorldSectorObserver()
{
}


WorldSectorObserver::~WorldSectorObserver()
{
}

void WorldSectorObserver::Load(Sector &sector)
{
  notify(&WorldSectorEvent::Load, sector);
}

void WorldSectorObserver::UnLoad(Sector &sector)
{
  notify(&WorldSectorEvent::UnLoad, sector);
}

void WorldSectorEvent::Load(Sector &sector)
{

}

void WorldSectorEvent::UnLoad(Sector &sector)
{

}
