#pragma once
#ifndef UpdatableArea_h__
#define UpdatableArea_h__


#include "tools\CoordSystem.h"
#include <vector>
#include <tuple>
#include "../RingBuffer.h"

class UpdatableSectors;
class World;

/// Зона обновления секторов.
/// Указывает миру, какие сектора нужно обновлять.
class UpdatableArea
{
public:
  void OnNewSector(SPos s);
  void OnDeleteSector(SPos s);
  UpdatableArea(World &world, const SPos &pos, int radius = 5);
  ~UpdatableArea();

  void SetPos(const SPos &pos);

  void SetRadius(int radius);

private:
  World &mWorld;
  SPos mPos;

  struct Positions
  {
    SPos pos;
    void init() {}
    void reset(SPos s) { pos = s; }
  };

  RingBuffer2D<Positions> mPositions;
private:
  boost::signals2::connection addCon, delCon;
};



#endif // UpdatableArea_h__
