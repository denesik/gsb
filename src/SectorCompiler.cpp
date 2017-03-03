#include "SectorCompiler.h"
#include "TesselatorSolidBlock.h"




SectorCompiler::SectorCompiler(const BlocksDataBase &dataBase)
  : mDataBase(dataBase)
{
  mVertexData.reserve(40000);
  mIndexData.reserve(50000);
  IndexType i = 0;
  for (SBPosType z = 1; z < TESSELATOR_SIZE - 1; ++z)
    for (SBPosType y = 1; y < TESSELATOR_SIZE - 1; ++y)
      for (SBPosType x = 1; x < TESSELATOR_SIZE - 1; ++x)
      {
        mIndex[i++] = cs::STtoTI({ x, y, z });
      }
  mBlocks.fill(0);

  mThread = std::thread([this]()
  {
    while (!mClose)
    {
      std::unique_lock<std::mutex> lock(mMutex);
      if (mRunned)
      {
        Process();
        mRunned = false;
      }
      mCv.wait(lock);
    }
  });
}

SectorCompiler::~SectorCompiler()
{
  mClose = true;
  mCv.notify_one();
  mThread.join();
}

void SectorCompiler::SetMiddle(const std::array<BlockId, SECTOR_CAPACITY> &data)
{
  for (size_t i = 0; i < SECTOR_CAPACITY; ++i)
  {
    mBlocks[mIndex[i]] = data[i];
  }
}

void SectorCompiler::Run()
{
  mRunned = true;
  mCv.notify_one();
}

bool SectorCompiler::IsDone() const
{
  return mRunned == false;
}

const std::vector<TesselatorVertex> & SectorCompiler::GetVertexData() const
{
  return mVertexData;
}

const std::vector<Magnum::UnsignedInt> & SectorCompiler::GetIndexData() const
{
  return mIndexData;
}

void SectorCompiler::ProcessSolidBlock(IndexType index, const STPos &pos)
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
  tesselator->PushBack(mVertexData, mIndexData, mIndexOffset, pos, static_cast<TesselatorSolidBlock::Side>(side));
}

void SectorCompiler::Process()
{
  mVertexData.clear();
  mIndexData.clear();
  mIndexOffset = 0;

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
