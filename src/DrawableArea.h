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

class World;

struct SectorRenderData
{
  Magnum::Buffer vertex_buffer;
  Magnum::Buffer index_buffer;
  Magnum::Mesh mesh;
  Magnum::Range3D aabb;
  Magnum::Matrix4 model;

  SectorRenderData();

  void SetPos(const SPos &pos);

  void Draw(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, Magnum::AbstractShaderProgram& shader);
};

// ������ ������� � ��������� �������.
// ������ ����� �� ��������� ������� � ������ ��� �� ����������!
class DrawableArea
{
public:
  DrawableArea(World &world, const SPos &pos, unsigned int radius = 5);
  ~DrawableArea();

  void SetRadius(unsigned int radius);

  void SetPos(const SPos &pos);

  void Draw(Camera &camera, Magnum::AbstractShaderProgram& shader);

private:
  World &mWorld;
  SPos mPos;
  
  // 0 - ������������� ���������� �������� ������ ������. ���������� ��� ��������� �������.
  // 1 - ���������� ���������� �������� � ����. ���������� ��� ��������� �������.
  // 2 - �������.
  // 3 - ���������� ��� �������.
  // 4 - ������ ������ �������.
  std::vector<std::tuple<SPos, SPos, std::weak_ptr<Sector>, std::shared_ptr<SectorCompiler>, std::unique_ptr<SectorRenderData>>> mSectors;
  Timer mTimer;

  std::shared_ptr<SectorCompiler> mSectorCompiler;
private:
  void UpdateRadius(unsigned int radius);
  void UpdatePos(const SPos &pos);

};



#endif // DrawableArea_h__
