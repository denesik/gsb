#pragma once
#ifndef WorldSectorObserver_h__
#define WorldSectorObserver_h__

#include "Observer.h"

class Sector;

class WorldSectorEvent
{
public:
  virtual ~WorldSectorEvent() = default;

  virtual void Load(Sector &sector);
  virtual void UnLoad(Sector &sector);
};

class WorldSectorObserver : public ptl::observer<WorldSectorEvent>
{
public:
  WorldSectorObserver();
  ~WorldSectorObserver();

  void Load(Sector &sector);
  void UnLoad(Sector &sector);

};



#endif // WorldSectorObserver_h__
