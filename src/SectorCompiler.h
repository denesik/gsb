#pragma once
#ifndef SectorTesselator_h__
#define SectorTesselator_h__

#include "StaticBlock.h"
#include <array>
#include "tools\CoordSystem.h"
#include <vector>
#include "Tesselator.h"
#include <Magnum/Magnum.h>
#include "DataBase.h"
#include "Block.h"
#include <memory>
#include "ThreadProcess.h"

class SectorCompiler : public ThreadProcess<SectorCompiler>
{
public:
  SectorCompiler(const DataBase &dataBase);
  ~SectorCompiler();

  void SetMiddle(const std::array<BlockId, SECTOR_CAPACITY> &data, const std::array<std::unique_ptr<TesselatorData>, SECTOR_CAPACITY> &tess_data);

  void SetSide(const std::array<BlockId, SECTOR_CAPACITY> &data, const std::array<std::unique_ptr<TesselatorData>, SECTOR_CAPACITY> &tess_data, SideFlags side);

  const std::vector<TesselatorVertex> &GetVertexData() const;
  const std::vector<Magnum::UnsignedInt> &GetIndexData() const;

  void Process();

private:
  std::array<BlockId, TESSELATOR_CAPACITY> mTesselators;
  std::array<TesselatorData, TESSELATOR_CAPACITY> mTesselatorsData;
  std::array<IndexType, SECTOR_CAPACITY> mIndexMiddle;
  std::array<IndexType, SECTOR_SIZE * SECTOR_HEIGHT> mIndexBlocks[6];
  std::array<IndexType, SECTOR_SIZE * SECTOR_HEIGHT> mIndexTess[6];

  std::vector<TesselatorVertex> mVertexData;
  std::vector<Magnum::UnsignedInt> mIndexData;
  Magnum::UnsignedInt mIndexOffset;

  const DataBase &mDataBase;
private:
  void ProcessSolidBlock(IndexType index, const STPos &pos);

  void ProcessMicroBlock(IndexType index, const STPos &pos);

  void GenerateIndex();
};



#endif // SectorTesselator_h__
