#pragma once
#ifndef UpdatableArea_h__
#define UpdatableArea_h__


#include "tools\CoordSystem.h"
#include <vector>
#include <tuple>

class UpdatableSectors;

class UpdatableArea
{
public:
  UpdatableArea(UpdatableSectors &updatable, const SPos &pos, unsigned int radius = 5);
  ~UpdatableArea();

  void SetPos(const SPos &pos);

  void SetRadius(unsigned int radius);

private:
  UpdatableSectors &mUpdatable;
  SPos mPos;

  std::vector<std::tuple<SPos, SPos>> mPositions;
private:
  void UpdateRadius(unsigned int radius);
  void UpdatePos(const SPos &pos);
};



#endif // UpdatableArea_h__
