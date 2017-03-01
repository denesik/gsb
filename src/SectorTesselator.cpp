#include "SectorTesselator.h"
#include "TesselatorSolidBlock.h"




SectorTesselator::SectorTesselator(const BlocksDataBase &dataBase)
  : mDataBase(dataBase)
{
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
        ProcessSolidBlock(cs::STtoTI(pos), pos);
      }
}

bool SectorTesselator::IsDone() const
{
  return true;
}

void SectorTesselator::ProcessSolidBlock(IndexType index, const STPos &pos)
{
  int side = TesselatorSolidBlock::NONE;

  if (mBlocks[cs::STtoTI(cs::Left(pos))] == 0)
    side |= TesselatorSolidBlock::LEFT;
  if (mBlocks[cs::STtoTI(cs::Right(pos))] == 0)
    side |= TesselatorSolidBlock::RIGHT;
  if (mBlocks[cs::STtoTI(cs::Top(pos))] == 0)
    side |= TesselatorSolidBlock::TOP;
  if (mBlocks[cs::STtoTI(cs::Bottom(pos))] == 0)
    side |= TesselatorSolidBlock::BOTTOM;
  if (mBlocks[cs::STtoTI(cs::Back(pos))] == 0)
    side |= TesselatorSolidBlock::BACK;
  if (mBlocks[cs::STtoTI(cs::Front(pos))] == 0)
    side |= TesselatorSolidBlock::FRONT;

  const auto *tesselator = static_cast<const TesselatorSolidBlock *>(mDataBase.GetBlockStaticPart(mBlocks[index])->GetTesselator().get());
  tesselator->PushBack(vertex_data, index_data, index_offset, pos, static_cast<TesselatorSolidBlock::Side>(side));
}
