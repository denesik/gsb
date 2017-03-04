#pragma once
#ifndef Sector_h__
#define Sector_h__

#include <Magnum/Magnum.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Mesh.h>
#include <Magnum/Buffer.h>

#include "BlockStaticPart.h"
#include <array>
#include "SectorCompiler.h"
#include "tools\CoordSystem.h"
#include <Magnum/AbstractShaderProgram.h>
#include <Magnum/Math/Frustum.h>
#include <Magnum/Math/Range.h>
#include <memory>

class IMapGenerator;
class World;

class Sector
{
public:
  Sector(World &morld, const SPos &pos);
  ~Sector();

  void CreateRenderData();

  bool NeedCompile() const;

  bool Compiling() const;

  void RunCompiler(std::shared_ptr<SectorCompiler> sectorCompiler);

  void Draw(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, Magnum::AbstractShaderProgram& shader);

  void Update();

  SPos GetPos() const;

  void ApplyGenerator(IMapGenerator &generator);

  BlockId GetBlockId(SBPos pos) const;
  void SetBlockId(SBPos pos, BlockId id);

private:
  std::array<BlockId, SECTOR_CAPACITY> mStaticBlocks;
  const SPos mPos;

  struct RenderData
  {
    Magnum::Buffer vertex_buffer;
    Magnum::Buffer index_buffer;
    Magnum::Mesh mesh;
  };

  std::unique_ptr<RenderData> mRenderData;

  Magnum::Range3D mAabb;
  Magnum::Matrix4 mModelMatrix;

  std::shared_ptr<SectorCompiler> mSectorCompiler;

public:
  World &mWorld;

  bool mNeedCompile = true;
};



#endif // Sector_h__
