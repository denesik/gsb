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

class World;

// ����� ����� ������� ����� ����� �������� ������.
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
  DrawableArea(World &world, const SPos &pos, unsigned int radius = 5);
  ~DrawableArea();

  void SetRadius(unsigned int radius);

  void SetPos(const SPos &pos);

  //TODO: �� ������������� ������, ���� �� ������������� � ������ ������.
  void Draw(const Camera &camera, const Camera &sun, const Vector3 &lightdir, StandartShader& shader);
  void DrawShadowPass(const Camera &sun, ShadowShader& shader);

private:
  World &mWorld;
  SPos mPos;

  struct Data
  {
    SPos local_pos;
    SPos world_pos;
    std::weak_ptr<Sector> sector;
    std::shared_ptr<SectorRenderData> drawable;
  };

  std::vector<Data> mData;
  Timer mTimer;

private:
  void UpdateRadius(unsigned int radius);
  void UpdatePos(const SPos &pos);

  ThreadWorker<TaskCompile, SectorCompiler> mCompilerWorker;
};



#endif // DrawableArea_h__
