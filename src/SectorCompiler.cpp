#include "SectorCompiler.h"
#include "TesselatorSolidBlock.h"
#include "TesselatorMicroBlock.h"
#include "TesselatorTexturedBlock.h"




SectorCompiler::SectorCompiler(const DataBase &dataBase)
  : mDataBase(dataBase)
{
  mVertexData.reserve(40000);
  mIndexData.reserve(50000);
  GenerateIndex();

  mTesselators.fill(0);
}

SectorCompiler::~SectorCompiler()
{
  Release();
}

void SectorCompiler::SetMiddle(const std::array<BlockId, gSectorCapacity> &data, const std::array<std::unique_ptr<TesselatorData>, gSectorCapacity> &tess_data)
{
  for (size_t i = 0; i < gSectorCapacity; ++i)
  {
    mTesselators[mIndexMiddle[i]] = data[i];
    if (tess_data[i])
      mTesselatorsData[mIndexMiddle[i]] = *tess_data[i];
  }
}

void SectorCompiler::SetSide(const std::array<BlockId, gSectorCapacity> &data, const std::array<std::unique_ptr<TesselatorData>, gSectorCapacity> &tess_data, SideFlags side)
{
  auto index = SideFlagIndex(side);
  for (SPosType i = 0; i < gSectorSize.x() * gSectorSize.y(); ++i)
  {
    mTesselators[mIndexTess[index][i]] = data[mIndexBlocks[index][i]];
    if (tess_data[mIndexBlocks[index][i]])
      mTesselatorsData[mIndexTess[index][i]] = *tess_data[mIndexBlocks[index][i]];
  }
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
    const auto &block = mDataBase.GetBlockStaticPart(mTesselators[cs::STtoTI(cs::Left(tpos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= SideFlags::LEFT;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mTesselators[cs::STtoTI(cs::Right(tpos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= SideFlags::RIGHT;
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
    const auto &block = mDataBase.GetBlockStaticPart(mTesselators[cs::STtoTI(cs::Back(tpos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= SideFlags::BACK;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mTesselators[cs::STtoTI(cs::Front(tpos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= SideFlags::FRONT;
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

  for (SBPosType z = 1; z < gTesselatorSize.z() - 1; ++z)
    for (SBPosType y = 1; y < gTesselatorSize.y() - 1; ++y)
      for (SBPosType x = 1; x < gTesselatorSize.x() - 1; ++x)
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

void SectorCompiler::GenerateIndex()
{
  IndexType i = 0;
  for (SBPosType z = 1; z < gTesselatorSize.z() - 1; ++z)
    for (SBPosType y = 1; y < gTesselatorSize.y() - 1; ++y)
      for (SBPosType x = 1; x < gTesselatorSize.x() - 1; ++x)
      {
        mIndexMiddle[i++] = cs::STtoTI({ x, y, z });
      }

  // left
  {
    auto side = SideFlagIndex(SideFlags::LEFT);
    i = 0;
    for (SBPosType z = 0; z < gSectorSize.z(); ++z)
      for (SBPosType y = 0; y < gSectorSize.y(); ++y)
      {
        mIndexBlocks[side][i++] = cs::SBtoBI({ 0, y, z });
      }
    i = 0;
    for (SBPosType z = 1; z < gTesselatorSize.z() - 1; ++z)
      for (SBPosType y = 1; y < gTesselatorSize.y() - 1; ++y)
      {
        mIndexTess[side][i++] = cs::STtoTI({ gTesselatorSize.x() - 1, y, z });
      }
  }
  // right
  {
    auto side = SideFlagIndex(SideFlags::RIGHT);
    i = 0;
    for (SBPosType z = 0; z < gSectorSize.z(); ++z)
      for (SBPosType y = 0; y < gSectorSize.y(); ++y)
      {
        mIndexBlocks[side][i++] = cs::SBtoBI({ gSectorSize.x() - 1, y, z });
      }
    i = 0;
    for (SBPosType z = 1; z < gTesselatorSize.z() - 1; ++z)
      for (SBPosType y = 1; y < gTesselatorSize.y() - 1; ++y)
      {
        mIndexTess[side][i++] = cs::STtoTI({ 0, y, z });
      }
  }

  // front
  {
    auto side = SideFlagIndex(SideFlags::FRONT);
    i = 0;
    for (SBPosType y = 0; y < gSectorSize.y(); ++y)
      for (SBPosType x = 0; x < gSectorSize.x(); ++x)
      {
        mIndexBlocks[side][i++] = cs::SBtoBI({ x, y, gSectorSize.z() - 1 });
      }
    i = 0;
    for (SBPosType y = 1; y < gTesselatorSize.y() - 1; ++y)
      for (SBPosType x = 1; x < gTesselatorSize.x() - 1; ++x)
      {
        mIndexTess[side][i++] = cs::STtoTI({ x, y, 0 });
      }
  }
  // back
  {
    auto side = SideFlagIndex(SideFlags::BACK);
    i = 0;
    for (SBPosType y = 0; y < gSectorSize.y(); ++y)
      for (SBPosType x = 0; x < gSectorSize.x(); ++x)
      {
        mIndexBlocks[side][i++] = cs::SBtoBI({ x, y, 0 });
      }
    i = 0;
    for (SBPosType y = 1; y < gTesselatorSize.y() - 1; ++y)
      for (SBPosType x = 1; x < gTesselatorSize.x() - 1; ++x)
      {
        mIndexTess[side][i++] = cs::STtoTI({ x, y, gTesselatorSize.z() - 1 });
      }
  }

  // top
  {
    auto side = SideFlagIndex(SideFlags::TOP);
    i = 0;
    for (SBPosType z = 0; z < gSectorSize.z(); ++z)
      for (SBPosType x = 0; x < gSectorSize.x(); ++x)
      {
        mIndexBlocks[side][i++] = cs::SBtoBI({ x, 0, z });
      }
    i = 0;
    for (SBPosType z = 1; z < gTesselatorSize.z() - 1; ++z)
      for (SBPosType x = 1; x < gTesselatorSize.x() - 1; ++x)
      {
        mIndexTess[side][i++] = cs::STtoTI({ x, gTesselatorSize.y() - 1, z });
      }
  }
  // bottom
  {
    auto side = SideFlagIndex(SideFlags::DOWN);
    i = 0;
    for (SBPosType z = 0; z < gSectorSize.z(); ++z)
      for (SBPosType x = 0; x < gSectorSize.x(); ++x)
      {
        mIndexBlocks[side][i++] = cs::SBtoBI({ x, gSectorSize.y() - 1, z });
      }
    i = 0;
    for (SBPosType z = 1; z < gTesselatorSize.z() - 1; ++z)
      for (SBPosType x = 1; x < gTesselatorSize.x() - 1; ++x)
      {
        mIndexTess[side][i++] = cs::STtoTI({ x, 0, z });
      }
  }
}
