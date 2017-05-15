#include "SectorCompiler.h"
#include "TesselatorSolidBlock.h"
#include "TesselatorMicroBlock.h"
#include "TesselatorTexturedBlock.h"

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

const std::vector<TesselatorVertex> & SectorCompiler::GetVertexData() const
{
  return mVertexData;
}

const std::vector<Magnum::UnsignedInt> & SectorCompiler::GetIndexData() const
{
  return mIndexData;
}

void SectorCompiler::ProcessSolidBlock(IndexType index, const STPos &tpos)
{
  // Рисовать сторону если тесселятор у смежного блока отсутствует или другого типа.
  int side = SideFlags::NONE;
  {
    const auto &block = mDataBase.GetBlockStaticPart(mTesselators[cs::STtoTI(cs::West(tpos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= SideFlags::WEST;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mTesselators[cs::STtoTI(cs::East(tpos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= SideFlags::EAST;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mTesselators[cs::STtoTI(cs::Top(tpos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= SideFlags::TOP;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mTesselators[cs::STtoTI(cs::Down(tpos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= SideFlags::DOWN;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mTesselators[cs::STtoTI(cs::North(tpos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= SideFlags::NORTH;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mTesselators[cs::STtoTI(cs::South(tpos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= SideFlags::SOUTH;
  }

  const auto *tesselator = static_cast<const TesselatorSolidBlock *>(mDataBase.GetBlockStaticPart(mTesselators[index])->GetTesselator().get());
  tesselator->PushBack(mVertexData, mIndexData, mIndexOffset, WPos(cs::TItoSB(index)), static_cast<SideFlags>(side));
}

void SectorCompiler::ProcessMicroBlock(IndexType index, const STPos &tpos)
{
  const auto *tesselator = static_cast<const TesselatorMicroBlock *>(mDataBase.GetBlockStaticPart(mTesselators[index])->GetTesselator().get());
  tesselator->PushBack(mTesselatorsData[index], mVertexData, mIndexData, mIndexOffset, WPos(cs::TItoSB(index)));
}

void SectorCompiler::ProcessTexturedBlock(IndexType index, const STPos &tpos)
{
  const auto *tesselator = static_cast<const TesselatorTexturedBlock *>(mDataBase.GetBlockStaticPart(mTesselators[index])->GetTesselator().get());
  tesselator->PushBack(mTesselatorsData[index], mVertexData, mIndexData, mIndexOffset, WPos(cs::TItoSB(index)));
}

void SectorCompiler::Process()
{
  mVertexData.clear();
  mIndexData.clear();
  mIndexOffset = 0;

  for (SBPosType z = 1; z < gBlockBatcherSize.z() - 1; ++z)
    for (SBPosType y = 1; y < gBlockBatcherSize.y() - 1; ++y)
      for (SBPosType x = 1; x < gBlockBatcherSize.x() - 1; ++x)
      {
        STPos pos{ x, y, z };
        auto index = cs::STtoTI(pos);
        const auto &block = mDataBase.GetBlockStaticPart(mTesselators[index]);
        if (block && block->GetTesselator())
        {
          if (block->GetTesselator()->Type() == Tesselator::TesselatorType::SOLID_BLOCK)
          {
            ProcessSolidBlock(index, pos);
          }
          if (block->GetTesselator()->Type() == Tesselator::TesselatorType::MICRO_BLOCK)
          {
            ProcessMicroBlock(index, pos);
          }
          if (block->GetTesselator()->Type() == Tesselator::TesselatorType::TEXTURED_BLOCK)
          {
            ProcessTexturedBlock(index, pos);
          }
        }
      }
  mTesselators.fill(0);
}
