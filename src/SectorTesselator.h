#pragma once
#ifndef SectorTesselator_h__
#define SectorTesselator_h__

#include "BlockStaticPart.h"
#include <array>
#include "tools\CoordSystem.h"
#include <vector>
#include "Tesselator.h"
#include <Magnum/Magnum.h>
#include "BlocksDataBase.h"

class SectorTesselator
{
public:
  SectorTesselator(const BlocksDataBase &dataBase);
  ~SectorTesselator();

  void SetMiddle(const std::array<BlockId, SECTOR_CAPACITY> &data);

  void Run();

  bool IsDone() const;

private:
  std::array<BlockId, TESSELATOR_CAPACITY> mBlocks;
  std::array<IndexType, SECTOR_CAPACITY> mIndex;

public:
  std::vector<TesselatorVertex> vertex_data;
  std::vector<Magnum::UnsignedInt> index_data;
  Magnum::UnsignedInt index_offset;

  const BlocksDataBase &mDataBase;

private:
  void ProcessSolidBlock(IndexType index, const STPos &pos);

};



#endif // SectorTesselator_h__
