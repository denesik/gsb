#include "SectorTesselator.h"
#include "TesselatorSolidBlock.h"




SectorTesselator::SectorTesselator(const BlocksDataBase &dataBase)
  : mDataBase(dataBase)
{
  vertex_data.reserve(40000);
  index_data.reserve(50000);
  IndexType i = 0;
  for (SBPosType z = 1; z < TESSELATOR_SIZE - 1; ++z)
    for (SBPosType y = 1; y < TESSELATOR_SIZE - 1; ++y)
      for (SBPosType x = 1; x < TESSELATOR_SIZE - 1; ++x)
      {
        mIndex[i++] = cs::STtoTI({ x, y, z });
      }
  mBlocks.fill(0);
}

SectorTesselator::~SectorTesselator()
{
}

void SectorTesselator::SetMiddle(const std::array<BlockId, SECTOR_CAPACITY> &data)
{
  for (size_t i = 0; i < SECTOR_CAPACITY; ++i)
  {
    mBlocks[mIndex[i]] = data[i];
  }
}

void SectorTesselator::Run()
{
  vertex_data.clear();
  index_data.clear();
  index_offset = 0;

  for (SBPosType z = 1; z < TESSELATOR_SIZE - 1; ++z)
    for (SBPosType y = 1; y < TESSELATOR_SIZE - 1; ++y)
      for (SBPosType x = 1; x < TESSELATOR_SIZE - 1; ++x)
      {
        STPos pos{ x, y, z };
        auto index = cs::STtoTI(pos);
        const auto &block = mDataBase.GetBlockStaticPart(mBlocks[index]);
        if (block && block->GetTesselator())
        {
          if (block->GetTesselator()->Type() == Tesselator::TesselatorType::SOLID_BLOCK)
          {
            ProcessSolidBlock(index, pos);
          }
        }
      }
}

bool SectorTesselator::IsDone() const
{
  return true;
}

void SectorTesselator::ProcessSolidBlock(IndexType index, const STPos &pos)
{
  // Рисовать сторону если тесселятор у смежного блока отсутствует или другого типа.
  int side = TesselatorSolidBlock::NONE;
  {
    const auto &block = mDataBase.GetBlockStaticPart(mBlocks[cs::STtoTI(cs::Left(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= TesselatorSolidBlock::LEFT;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mBlocks[cs::STtoTI(cs::Right(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= TesselatorSolidBlock::RIGHT;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mBlocks[cs::STtoTI(cs::Top(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= TesselatorSolidBlock::TOP;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mBlocks[cs::STtoTI(cs::Bottom(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= TesselatorSolidBlock::BOTTOM;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mBlocks[cs::STtoTI(cs::Back(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= TesselatorSolidBlock::BACK;
  }
  {
    const auto &block = mDataBase.GetBlockStaticPart(mBlocks[cs::STtoTI(cs::Front(pos))]);
    if (!block || !block->GetTesselator() || block->GetTesselator()->Type() != Tesselator::TesselatorType::SOLID_BLOCK)
      side |= TesselatorSolidBlock::FRONT;
  }

  const auto *tesselator = static_cast<const TesselatorSolidBlock *>(mDataBase.GetBlockStaticPart(mBlocks[index])->GetTesselator().get());
  tesselator->PushBack(vertex_data, index_data, index_offset, pos, static_cast<TesselatorSolidBlock::Side>(side));
}
