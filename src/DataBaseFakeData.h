#pragma once
#ifndef DataBaseFakeData_h__
#define DataBaseFakeData_h__

#include "World.h"
#include "Sector.h"

class DataBase;

struct FakeData
{
  FakeData(const DataBase &db)
    : world(db), sector(world, SPos{})
  {

  }
  World world;
  Sector sector;
};

#endif // DataBaseFakeData_h__
