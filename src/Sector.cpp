#include "Sector.h"
#include "World.h"
#include "MapGenerator.h"
#include "TesselatorMicroBlock.h"

using namespace Magnum;

Sector::Sector(World &world, const SPos &pos)
  : mWorld(world), mPos(pos)
{
  // generate sector
  mStaticBlocks.fill(2);
//   mStaticBlocks[cs::SBtoBI({0, 31, 0})] = 3;
//   mDinamicBlocks[cs::SBtoBI({ 0, 31, 0 })] = std::make_unique<BlockDinamicPart>();
//   mDinamicBlocks[cs::SBtoBI({ 0, 31, 0 })]->mTesselatorData = std::make_unique<TesselatorData>();
//   auto &data = TesselatorMicroBlock::ToMicroblockData(*(mDinamicBlocks[cs::SBtoBI({ 0, 31, 0 })]->mTesselatorData));
// 
//   data[TesselatorMicroBlock::ToIndex({0, 2, 0})] = 1;
}


Sector::~Sector()
{
}

bool Sector::NeedCompile() const
{
  return mNeedCompile;
}

void Sector::NeedCompile(bool value)
{
  mNeedCompile = value;
}

void Sector::SetCompilerData(SectorCompiler &sectorCompiler)
{
  sectorCompiler.SetMiddle(mStaticBlocks, mDinamicBlocks);

  if (auto sector = mWorld.GetSector(cs::Left(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mDinamicBlocks, SideFlags::LEFT);

  if (auto sector = mWorld.GetSector(cs::Front(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mDinamicBlocks, SideFlags::FRONT);

  if (auto sector = mWorld.GetSector(cs::Top(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mDinamicBlocks, SideFlags::TOP);

  if (auto sector = mWorld.GetSector(cs::Right(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mDinamicBlocks, SideFlags::RIGHT);

  if (auto sector = mWorld.GetSector(cs::Back(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mDinamicBlocks, SideFlags::BACK);

  if (auto sector = mWorld.GetSector(cs::Bottom(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mDinamicBlocks, SideFlags::BOTTOM);
}

void Sector::Update()
{

}

SPos Sector::GetPos() const
{
  return mPos;
}

void Sector::ApplyGenerator(IMapGenerator &generator)
{
  generator.Generate(*this);
}

BlockId Sector::GetBlockId(SBPos pos) const
{
  return mStaticBlocks[cs::SBtoBI(pos)];
}

void Sector::SetBlockId(SBPos pos, BlockId id)
{
  mStaticBlocks[cs::SBtoBI(pos)] = id;
  mNeedCompile = true;
}

void Sector::SetBlockDynamic(SBPos pos, std::unique_ptr<BlockDinamicPart> dyn)
{
  mDinamicBlocks[cs::SBtoBI(pos)] = std::move(dyn);
}

BlockDinamicPart& Sector::GetBlockDynamic(SBPos pos)
{
  return *mDinamicBlocks[cs::SBtoBI(pos)];
}
