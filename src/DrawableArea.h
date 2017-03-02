#pragma once
#ifndef DrawableArea_h__
#define DrawableArea_h__

#include "tools\CoordSystem.h"
#include <memory>
#include <tuple>
#include "Sector.h"
#include <Magnum\AbstractShaderProgram.h>
#include "Timer.h"
#include <Magnum\Magnum.h>


class World;

// Рисуем сектора в указанной области.
// Данный класс не загружает сектора в память при их отсутствии!
class DrawableArea
{
public:
  DrawableArea(World &world, const SPos &pos, unsigned int radius = 5);
  ~DrawableArea();

  void SetRadius(unsigned int radius);

  void SetPos(const SPos &pos);

  void Draw(const Magnum::Matrix4 &matrix, Magnum::AbstractShaderProgram& shader);

private:
  World &mWorld;
  SPos mPos;
  
  // 1 - Относительные координаты секторов вокруг центра. Изменяются при изменении радиуса.
  // 2 - Глобальные координаты секторов в мире. Изменяются при изменении позиции.
  // 3 - Сектора.
  std::vector<std::tuple<SPos, SPos, std::weak_ptr<Sector>>> mSectors;
  Timer mTimer;
private:
  void UpdateRadius(unsigned int radius);
  void UpdatePos(const SPos &pos);

};



#endif // DrawableArea_h__
