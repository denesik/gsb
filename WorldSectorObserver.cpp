#include "WorldSectorObserver.h"
#include "src\Log.h"
#include "src\Sector.h"


static int sector_count = 0;

WorldSectorObserver::WorldSectorObserver()
{
}


WorldSectorObserver::~WorldSectorObserver()
{
}

void WorldSectorObserver::Load(Sector &sector)
{
  notify(&WorldSectorEvent::Load, sector);
  ++sector_count;
  //LOG(trace) << "Load sector [" << sector.GetPos().x() << " " << sector.GetPos().z() << "]. Count: " << sector_count;
}

void WorldSectorObserver::UnLoad(Sector &sector)
{
  notify(&WorldSectorEvent::UnLoad, sector);
  --sector_count;
  //LOG(trace) << "Unload sector [" << sector.GetPos().x() << " " << sector.GetPos().z() << "]. Count: " << sector_count;
}

void WorldSectorEvent::Load(Sector &sector)
{

}

void WorldSectorEvent::UnLoad(Sector &sector)
{

}
