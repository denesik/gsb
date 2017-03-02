#pragma once
#ifndef UpdatableSectors_h__
#define UpdatableSectors_h__


#include "tools\CoordSystem.h"
#include <vector>
#include <tuple>
#include <memory>
#include "Sector.h"
#include "Timer.h"

class World;

class UpdatableSectors
{
public:
  UpdatableSectors(World &world);
  ~UpdatableSectors();

  void Add(const SPos &pos);

  void Remove(const SPos &pos);

  void Update();

private:
  std::vector<std::tuple<SPos, size_t, std::weak_ptr<Sector>>> mSectors;
  Timer mTimer;
  World &mWorld;

};



#endif // UpdatableSectors_h__
