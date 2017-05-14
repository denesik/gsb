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
#include "SectorCompiler.h"

#include <Magnum/Math/Matrix4.h>
#include <Magnum/Mesh.h>
#include <Magnum/Buffer.h>
#include <Magnum/Math/Range.h>
#include <Magnum/Math/Matrix4.h>
#include "Camera.h"
#include "ThreadWorker.h"
#include <StandartShader.h>
#include "../RingBuffer.h"

class World;

// Нужен класс который будет уметь рисовать сектор.
// 

struct SectorRenderData
{
  bool isValid = false;
  bool isCompiling = false;

  Magnum::Buffer vertex_buffer;
  Magnum::Buffer index_buffer;
  Magnum::Mesh mesh;
  Magnum::Range3D aabb;
  Magnum::Matrix4 model;

  SectorRenderData();

  void SetPos(const SPos &pos);

  void Draw(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, SunCamera & sun_matrix, const Magnum::Vector3 &lightdir, StandartShader& shader);
  void DrawShadowPass(const Magnum::Frustum &frustum, const Magnum::Matrix4 & sun_matrix, ShadowShader& shader);
};


class TaskCompile
{
public:
  TaskCompile(std::weak_ptr<Sector> sector, std::weak_ptr<SectorRenderData> drawable);

  bool Begin(SectorCompiler &compiler);

  void End(const SectorCompiler &compiler);

private:
  std::weak_ptr<Sector> mSector;
  std::weak_ptr<SectorRenderData> mDrawable;
  SPos mPos;
};

// Список на обновление.
// Проверяем есть ли воркер.
// Если нету - ищем подходящий воркер. ??
// Если есть проверяем завершил ли воркер работу.
// Если завершил - вызываем обвновление данных, удаляем элемент, удаляем воркер.
// Если нету но нашли - вызываем загрузку данных в воркер.
// Если не смогли загрузить - удаляем элемент, удаляем воркер.

// Рисуем сектора в указанной области.
// Данный класс не загружает сектора в память при их отсутствии!
class DrawableArea
{
public:
  DrawableArea(World &world, const SPos &pos, int radius = 5);
  ~DrawableArea();

  void SetRadius(int radius);

  void SetPos(const SPos &pos);

  //TODO: Не компилировать сектор, если он компилируется в данный момент.
  void Draw(const ICamera &camera, SunCamera &sun, const Magnum::Vector3 &lightdir, StandartShader& shader);
  void DrawShadowPass(const ICamera &sun, ShadowShader& shader);

private:
  World &mWorld;
  SPos mPos;

  struct Data
  {
    SPos world_pos;
    std::weak_ptr<Sector> sector;
    std::shared_ptr<SectorRenderData> drawable;

    void init();
    void reset(SPos pos);
  };

  RingBuffer2D<Data> mData;
  Timer mTimer;

private:

  ThreadWorker<TaskCompile, SectorCompiler> mCompilerWorker;
};



#endif // DrawableArea_h__
