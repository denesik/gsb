#include "SectorCompiler.h"
#include "TesselatorSolidBlock.h"
#include "TesselatorMicroBlock.h"
#include "TesselatorTexturedBlock.h"
#include "TesselatorFlatBlock.h"

SectorCompiler::SectorCompiler(const DataBase &dataBase)
  : mDataBase(dataBase)
{
  mVertexData.reserve(40000);
  mIndexData.reserve(50000);

  mTesselators.fill(0);
}

SectorCompiler::~SectorCompiler()
{
  //Release();
}

void SectorCompiler::Process()
{
  mVertexData.clear();
  mIndexData.clear();

  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
      for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
      {
        STPos pos{ x, y, z };
        auto index = cs::STtoTI(pos);
        const auto &block = mDataBase.GetBlockStaticPart(mTesselators[index]);
        if (block && block->GetTesselator())
        {
          block->GetTesselator()->Process(*this, pos);
        }
      }
  mTesselators.fill(0);
}
