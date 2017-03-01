#pragma once
#ifndef DrawableArea_h__
#define DrawableArea_h__

#include "tools\CoordSystem.h"
#include <memory>
#include <tuple>
#include "Sector.h"
#include <Magnum\AbstractShaderProgram.h>

class World;

// –исуем сектора в указанной области.
// ƒанный класс не загружает сектора в пам€ть при их отсутствии!
class DrawableArea
{
public:
  DrawableArea(World *world, const SPos &pos, unsigned int radius = 5);
  ~DrawableArea();

  void SetPos(const SPos &pos);

  void Draw(Magnum::AbstractShaderProgram& shader);

private:
  World *mWorld;
  SPos mPos;

  std::vector<std::tuple<SPos, SPos, std::weak_ptr<Sector>>> mSectors;

private:
  void UpdateRadius(unsigned int radius);
  void UpdatePos(const SPos &pos);

};



#endif // DrawableArea_h__
