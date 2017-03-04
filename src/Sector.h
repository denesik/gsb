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
#include "BlockDinamicPart.h"

class IMapGenerator;
class World;

class Sector
{
public:
  Sector(World &morld, const SPos &pos);
  ~Sector();

  bool NeedCompile() const;

  void NeedCompile(bool value);

  void SetCompilerData(SectorCompiler &sectorCompiler);

  void Update();

  SPos GetPos() const;

  void ApplyGenerator(IMapGenerator &generator);

private:
  std::array<BlockId, SECTOR_CAPACITY> mStaticBlocks;
  std::array<std::unique_ptr<BlockDinamicPart>, SECTOR_CAPACITY> mDinamicBlocks;
  const SPos mPos;

  World &mWorld;

  bool mNeedCompile = true;
};



#endif // Sector_h__
