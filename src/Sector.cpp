#include "Sector.h"



Sector::Sector(const BlocksDataBase &dataBase)
  : mSectorTesselator(dataBase)
{
  mStaticBlocks.fill(0);

  // generate sector
  mStaticBlocks.fill(2);
}


Sector::~Sector()
{
}

void Sector::RunTesselator()
{
  mSectorTesselator.SetMiddle(mStaticBlocks);
  mSectorTesselator.Run();
}
