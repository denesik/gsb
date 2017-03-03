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

class World;

class Sector
{
public:
  Sector(World &morld, const SPos &pos);
  ~Sector();

  bool NeedCompile() const;

  bool Compiling() const;

  void RunCompiler(std::shared_ptr<SectorCompiler> sectorCompiler);

  void Draw(const Magnum::Frustum &frustum, const Magnum::Matrix4 &matrix, Magnum::AbstractShaderProgram& shader);

  void Update();

private:
  std::array<BlockId, SECTOR_CAPACITY> mStaticBlocks;
  const SPos mPos;

  Magnum::Range3D mAabb;
  Magnum::Matrix4 mModelMatrix;
  Magnum::Buffer mVertexBuffer;
  Magnum::Buffer mIndexBuffer;
  Magnum::Mesh mMesh;

  std::shared_ptr<SectorCompiler> mSectorCompiler;

public:
  World &mWorld;

  bool mNeedCompile = true;
};



#endif // Sector_h__
