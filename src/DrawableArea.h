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

// ����� ����� ������� ����� ����� �������� ������.
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

// ������ �� ����������.
// ��������� ���� �� ������.
// ���� ���� - ���� ���������� ������. ??
// ���� ���� ��������� �������� �� ������ ������.
// ���� �������� - �������� ����������� ������, ������� �������, ������� ������.
// ���� ���� �� ����� - �������� �������� ������ � ������.
// ���� �� ������ ��������� - ������� �������, ������� ������.

// ������ ������� � ��������� �������.
// ������ ����� �� ��������� ������� � ������ ��� �� ����������!
class DrawableArea
{
public:
  DrawableArea(World &world, const SPos &pos, int radius = 5);
  ~DrawableArea();

  void SetRadius(int radius);

  void SetPos(const SPos &pos);

  //TODO: �� ������������� ������, ���� �� ������������� � ������ ������.
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
