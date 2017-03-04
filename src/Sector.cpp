#include "Sector.h"
#include "World.h"
#include "MapGenerator.h"

using namespace Magnum;

Sector::Sector(World &world, const SPos &pos)
  : mWorld(world), mPos(pos)
{
  // generate sector
  mStaticBlocks.fill(2);
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
  sectorCompiler.SetMiddle(mStaticBlocks);

  if (auto sector = mWorld.GetSector(cs::Left(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, SideFlags::LEFT);

  if (auto sector = mWorld.GetSector(cs::Front(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, SideFlags::FRONT);

  if (auto sector = mWorld.GetSector(cs::Top(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, SideFlags::TOP);

  if (auto sector = mWorld.GetSector(cs::Right(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, SideFlags::RIGHT);

  if (auto sector = mWorld.GetSector(cs::Back(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, SideFlags::BACK);

  if (auto sector = mWorld.GetSector(cs::Bottom(mPos)).lock())
    sectorCompiler.SetSide(sector->mStaticBlocks, SideFlags::BOTTOM);
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

  generator.Generate(*this, mStaticBlocks);
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
