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
#include <boost\optional\optional.hpp>
#include "RingBuffer.h"
#include "ThreadProcess.h"

class World;

// Нужен класс который будет уметь рисовать сектор.
// 

struct SectorRenderData
{
  bool valide = false;
  bool compile = false;

  Magnum::Buffer vertex_buffer;
  Magnum::Buffer index_buffer;
  Magnum::Mesh mesh;
  Magnum::Range3D aabb;
  Magnum::Matrix4 model;

  SectorRenderData();

  void SetPos(const SPos &pos);

  void Draw(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, const Magnum::Matrix4 &sun_matrix, const Magnum::Vector3 &lightdir, StandartShader& shader);
  void DrawShadowPass(const Magnum::Frustum &frustum, const Magnum::Matrix4 &sun_matrix, ShadowShader& shader);
};


class DrawableArea
{
public:
  DrawableArea(World &world, const SPos &pos, unsigned int radius = 1);
  ~DrawableArea();

  void SetRadius(unsigned int radius);

  void SetPos(const SPos &pos);

  //TODO: Не компилировать сектор, если он компилируется в данный момент.
  void Draw(const Camera &camera, const Camera &sun, const Vector3 &lightdir, StandartShader& shader);
  //void DrawShadowPass(const Camera &sun, ShadowShader& shader);

private:
  World &mWorld;
  SPos mPos;

  struct Data
  {
    boost::optional<Sector &> sector;
    SectorRenderData drawable;
  };

  RingBuffer2D<Data> mData;
  Timer mTimer;

  struct Worker
  {
    Worker(const DataBase &dataBase)
      : compiler(dataBase)
    {}

    void Process(Sector &sector)
    {
      compiler.Process();
    }

    SectorCompiler compiler;
  };

  ThreadProcess<Worker, SPos> mCompiler;

private:
  void OnSectorLoadBegin(Worker &worker, SPos &pos);
  void OnSectorLoadEnd(Worker &worker, SPos &pos);

  void OnSectorAdd(const SPos &pos);
  void OnSectorRemove(const SPos &pos);
};



#endif // DrawableArea_h__
