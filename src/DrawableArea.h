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
#include <unordered_map>
#include <type_traits>
#include "..\WorldSectorObserver.h"

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

  SPos m_pos;

  SectorRenderData();
  SectorRenderData(SectorRenderData &&other) = default;
  SectorRenderData &operator=(SectorRenderData &&) = default;

  void SetPos(const SPos &pos);

  void Draw(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, const Magnum::Matrix4 &sun_matrix, const Magnum::Vector3 &lightdir, StandartShader& shader);
  void DrawShadowPass(const Magnum::Frustum &frustum, const Magnum::Matrix4 &sun_matrix, ShadowShader& shader);
};


class DrawableArea : public WorldSectorEvent
{
public:
  DrawableArea(World &world);
  ~DrawableArea();

  void SetRadius(unsigned int radius);

  void SetPos(const SPos &pos);

  //TODO: Не компилировать сектор, если он компилируется в данный момент.
  void Draw(const Camera &camera, const Camera &sun, const Vector3 &lightdir, StandartShader& shader);
  //void DrawShadowPass(const Camera &sun, ShadowShader& shader);

private:
  World &mWorld;
  SPos mPos;

  struct SectorData
  {
    SectorData() = default;
    SectorData(const SPos &_pos)
      : pos(_pos)
    {}

    const SPos pos;
    boost::optional<Sector &> sector;
    size_t count = 0;
    bool added = false;
  };

  struct BufferData
  {
    BufferData(SectorData &_sector)
      : sector_data(_sector)
    {}
    BufferData(BufferData &&other) = default;
    BufferData &operator=(BufferData &&) = default;

    std::reference_wrapper<SectorData> sector_data;
    SectorRenderData drawable;
  };

  std::unordered_map<SPos, SectorData> mSectors;
  RingBuffer<BufferData> mBufferData;
  Timer mTimer;

  struct Worker
  {
    Worker(const DataBase &dataBase)
      : compiler(dataBase)
    {}

    void Process(SPos &pos)
    {
      compiler.Process();
    }

    SectorCompiler compiler;
  };

  ThreadProcess<Worker, SPos> mCompiler;

private:
  void Load(Sector &sector) override;
  void UnLoad(Sector &sector) override;

  BufferData OnBufferAdd(const SPos &pos);
  void OnBufferRemove(BufferData &data, const SPos &pos);

  bool OnCompilerBegin(Worker &worker, SPos &pos);
  bool OnCompilerEnd(Worker &worker, SPos &pos);

  void UseSector(const SPos &pos);
  void UnuseSector(const SPos &pos);

  SectorData &GetSectorData(const SPos &pos);
};



#endif // DrawableArea_h__
