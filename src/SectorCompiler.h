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

class SectorCompiler/* : public ThreadProcess_old<SectorCompiler>*/
{
public:
  SectorCompiler(const DataBase &dataBase);
  ~SectorCompiler();

  const std::vector<TesselatorVertex> &GetVertexData() const
  {
    return mVertexData;
  }

  std::vector<TesselatorVertex> &GetVertexData()
  {
    return mVertexData;
  }

  const std::vector<Magnum::UnsignedInt> &GetIndexData() const
  {
    return mIndexData;
  }

  std::vector<Magnum::UnsignedInt> &GetIndexData()
  {
    return mIndexData;
  }

  void Process();

  inline std::array<BlockId, gBlockBatcherCapacity> &Tesselators()
  {
    return mTesselators;
  }

  inline std::array<Tesselator::Data, gBlockBatcherCapacity> &TesselatorsData()
  {
    return mTesselatorsData;
  }

  const DataBase &GetDataBase() const
  {
    return mDataBase;
  }

private:
  std::array<BlockId, gBlockBatcherCapacity> mTesselators;
  std::array<Tesselator::Data, gBlockBatcherCapacity> mTesselatorsData;

  std::vector<TesselatorVertex> mVertexData;
  std::vector<Magnum::UnsignedInt> mIndexData;

  const DataBase &mDataBase;
private:
  void ProcessSolidBlock(IndexType index, const STPos &pos);
  void ProcessMicroBlock(IndexType index, const STPos &pos);
  void ProcessFlatBlock(IndexType index, const STPos & tpos);
  void ProcessTexturedBlock(IndexType index, const STPos &pos);
};



#endif // SectorTesselator_h__
