#pragma once
#ifndef UpdatableArea_h__
#define UpdatableArea_h__


#include "tools\CoordSystem.h"
#include <vector>
#include <tuple>
#include "RingBuffer.h"

class UpdatableSectors;
class World;

/// Зона обновления секторов.
/// Указывает миру, какие сектора нужно обновлять.
// class UpdatableArea
// {
// public:
//   UpdatableArea(World &world, const SPos &pos, unsigned int radius = 5);
//   ~UpdatableArea();
// 
//   void SetPos(const SPos &pos);
// 
//   void SetRadius(unsigned int radius);
// 
// private:
//   World &mWorld;
//   SPos mPos;
// 
//   std::vector<std::tuple<SPos, SPos>> mPositions;
// private:
//   void UpdateRadius(unsigned int radius);
//   void UpdatePos(const SPos &pos);
// };


class SectorLoader
{
public:
  SectorLoader(World &world);
  ~SectorLoader();

  void SetPos(const SPos &pos);

  void SetRadius(unsigned int radius);

private:
  RingBuffer<int> mBufferData;
  World &mWorld;

private:
};


#endif // UpdatableArea_h__
