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

class World;

class Sector
{
public:
  Sector(World *morld);
  ~Sector();

  bool NeedCompile() const;

  void RunCompiler();

  void Draw(Magnum::AbstractShaderProgram& shader);

private:
  std::array<BlockId, SECTOR_CAPACITY> mStaticBlocks;

  Magnum::Matrix4 mModel;
  Magnum::Buffer mVertexBuffer;
  Magnum::Buffer mIndexBuffer;
  Magnum::Mesh mMesh;

public:
  SectorCompiler mSectorTesselator;

  World *mWorld;

  bool mNeedCompile = true;
};



#endif // Sector_h__
