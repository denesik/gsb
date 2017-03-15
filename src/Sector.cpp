#include "Sector.h"
#include "World.h"
#include "MapGenerator.h"
#include "TesselatorMicroBlock.h"
#include <Magnum/Timeline.h>

using namespace Magnum;

Sector::Sector(World &world, const SPos &pos)
  : mWorld(world), mPos(pos)
{
  // generate sector
  mStaticBlocks.fill(0);
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
  sectorCompiler.SetMiddle(mStaticBlocks, mTesselatorData);

  if (auto sector = mWorld.GetSector(cs::Left(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mTesselatorData, SideFlags::LEFT);

  if (auto sector = mWorld.GetSector(cs::Front(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mTesselatorData, SideFlags::FRONT);

  if (auto sector = mWorld.GetSector(cs::Top(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mTesselatorData, SideFlags::TOP);

  if (auto sector = mWorld.GetSector(cs::Right(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mTesselatorData, SideFlags::RIGHT);

  if (auto sector = mWorld.GetSector(cs::Back(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mTesselatorData, SideFlags::BACK);

  if (auto sector = mWorld.GetSector(cs::Bottom(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, sector->mTesselatorData, SideFlags::BOTTOM);
}

void Sector::Update()
{

}

SPos Sector::GetPos() const
{
  return mPos;
}

World & Sector::GetWorld()
{
  return mWorld;
}

void Sector::ApplyGenerator(IMapGenerator &generator)
{
  generator.Generate(*this);
}

BlockId Sector::GetBlockId(SBPos pos) const
{
  return mStaticBlocks[cs::SBtoBI(pos)];
}

Block* Sector::GetBlockDynamic(const WBPos& pos) const
{
  return mDinamicBlocks[cs::SBtoBI(pos)].get();
}

void Sector::CreateBlock(SBPos pos, BlockId id)
{
  auto block = mWorld.GetBlocksDataBase().CreateBlock(*this, id);
  mStaticBlocks[cs::SBtoBI(pos)] = std::get<0>(block);
  mTesselatorData[cs::SBtoBI(pos)] = std::move(std::get<1>(block));
  mDinamicBlocks[cs::SBtoBI(pos)] = std::move(std::get<2>(block));
  mNeedCompile = true;

  if (mDinamicBlocks[cs::SBtoBI(pos)])
  {
    mDinamicBlocks[cs::SBtoBI(pos)]->SetPos(cs::SBtoBI(pos));
  }
}

std::optional<TesselatorData &> Sector::GetTesselatorData(SBPos pos)
{
  return *mTesselatorData[cs::SBtoBI(pos)];
}
