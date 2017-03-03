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
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>  

class SectorCompiler
{
public:
  SectorCompiler(const BlocksDataBase &dataBase);
  ~SectorCompiler();

  void SetMiddle(const std::array<BlockId, SECTOR_CAPACITY> &data);

  void SetSide(const std::array<BlockId, SECTOR_CAPACITY> &data, SideFlags side);

  void Run();

  bool IsDone() const;

  const std::vector<TesselatorVertex> &GetVertexData() const;
  const std::vector<Magnum::UnsignedInt> &GetIndexData() const;

private:
  std::array<BlockId, TESSELATOR_CAPACITY> mTesselators;
  std::array<IndexType, SECTOR_CAPACITY> mIndexMiddle;
  std::array<IndexType, SECTOR_SIZE * SECTOR_SIZE> mIndexBlocks[6];
  std::array<IndexType, SECTOR_SIZE * SECTOR_SIZE> mIndexTess[6];

  std::vector<TesselatorVertex> mVertexData;
  std::vector<Magnum::UnsignedInt> mIndexData;
  Magnum::UnsignedInt mIndexOffset;

  const BlocksDataBase &mDataBase;

  std::thread mThread;
  std::atomic<bool> mRunned = false;
  std::atomic<bool> mClose = false;
  std::condition_variable mCv;
  std::mutex mMutex;
private:
  void ProcessSolidBlock(IndexType index, const STPos &pos);

  void Process();

  void GenerateIndex();
};



#endif // SectorTesselator_h__
