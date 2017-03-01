#pragma once
#ifndef Sector_h__
#define Sector_h__

#include <Magnum/Magnum.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Mesh.h>
#include <Magnum/Buffer.h>

#include "BlockStaticPart.h"
#include <array>
#include "SectorTesselator.h"
#include "tools\CoordSystem.h"


class Sector
{
public:
  Sector(const BlocksDataBase &dataBase);
  ~Sector();

  void RunTesselator();

private:
  std::array<BlockId, SECTOR_CAPACITY> mStaticBlocks;

  Magnum::Matrix4 mModel;
  Magnum::Buffer mVertexBuffer;
  Magnum::Buffer mIndexBuffer;
  Magnum::Mesh mMesh;

public:
  SectorTesselator mSectorTesselator;
};



#endif // Sector_h__
