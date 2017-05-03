#pragma once
#ifndef WorldSectorObserver_h__
#define WorldSectorObserver_h__

#include "Observer.h"
#include <tools\Common.h>

class Sector;

class GSB_NOVTABLE IWorldSectorEvent
{
public:
  virtual ~IWorldSectorEvent();
  virtual void Load(Sector &sector) = 0;
  virtual void UnLoad(Sector &sector) = 0;
};

class WorldSectorObserver : public ptl::observer<IWorldSectorEvent>
{
public:
  WorldSectorObserver();
  ~WorldSectorObserver();

  void Load(Sector &sector);
  void UnLoad(Sector &sector);

};



#endif // WorldSectorObserver_h__
