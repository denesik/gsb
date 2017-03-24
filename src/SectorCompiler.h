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

class SectorCompiler /*: public ThreadProcess<SectorCompiler>*/
{
public:
  SectorCompiler(const DataBase &dataBase);
  ~SectorCompiler();

  const std::vector<TesselatorVertex> &GetVertexData() const;
  const std::vector<Magnum::UnsignedInt> &GetIndexData() const;

  void Process();

  inline std::array<BlockId, gBlockBatcherCapacity> &Tesselators()
  {
    return mTesselators;
  }

  inline std::array<TesselatorData, gBlockBatcherCapacity> &TesselatorsData()
  {
    return mTesselatorsData;
  }

  inline void SetPos(const SPos &pos)
  {
    mPos = pos;
  }

  inline SPos GetPos() const
  {
    return mPos;
  }

private:
  std::array<BlockId, gBlockBatcherCapacity> mTesselators;
  std::array<TesselatorData, gBlockBatcherCapacity> mTesselatorsData;

  std::vector<TesselatorVertex> mVertexData;
  std::vector<Magnum::UnsignedInt> mIndexData;
  Magnum::UnsignedInt mIndexOffset;

  const DataBase &mDataBase;

  SPos mPos;
private:
  void ProcessSolidBlock(IndexType index, const STPos &pos);

  void ProcessMicroBlock(IndexType index, const STPos &pos);
  void ProcessTexturedBlock(IndexType index, const STPos &pos);
};



#endif // SectorTesselator_h__
