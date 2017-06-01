#include "Sector.h"
#include "World.h"
#include "TesselatorMicroBlock.h"
#include <Magnum/Timeline.h>
#include <assert.h>

using namespace Magnum;

Sector::Sector(World &world, const SPos &pos)
  : mWorld(world), mPos(pos), mChunk(*this),
  mSectorAround(world.FakeSector())
{
  // generate sector
  mStaticBlocks.fill(1);
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
  mChunk.SetCompilerData(sectorCompiler);
}

void Sector::Update()
{
  // Кешируем соседние сектора.
//   for (size_t i = 0; i < mSectorAround.sectors.size(); ++i)
//   {
//     if (mSectorAround.sectors[i].expired())
//     {
//       mSectorAround.sectors[i] = mWorld.GetSector(mPos + SectorAround::pos[i]);
//     }
//   }
}

void Sector::Draw(const Magnum::Matrix4 &vp, Magnum::AbstractShaderProgram& shader)
{
  for (auto &block : mDinamicBlocks)
  {
    if (block)
    {
      block->Draw(vp, shader);
    }
  }
}

SPos Sector::GetPos() const
{
  return mPos;
}

World & Sector::GetWorld()
{
  return mWorld;
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

std::optional<Tesselator::Data &> Sector::GetTesselatorData(SBPos pos)
{
  return *mTesselatorData[cs::SBtoBI(pos)];
}

// void Sector::LoadSector(Sector &sector)
// {
//   auto pos = sector.GetPos() - mPos;
//   if (mSectorAround.inside(pos))
//   {
//     mSectorAround.sectors[mSectorAround.to_index(pos)] = sector;
//   }
// }
// 
// void Sector::UnloadSector(Sector &sector)
// {
//   auto pos = sector.GetPos() - mPos;
//   if (mSectorAround.inside(pos))
//   {
//     mSectorAround.sectors[mSectorAround.to_index(pos)] = mWorld.FakeSector();
//   }
// }

void Sector::Cache(Sector &sector, CacheState state)
{
  auto pos = sector.GetPos() - mPos;
  if (mSectorAround.inside(pos))
  {
    mSectorAround.sectors[mSectorAround.to_index(pos)] = state == CACHE_LOAD ? sector : mWorld.FakeSector();
  }
}

void Sector::FlatbufferLoad(const gsb_flat::Sector & sec)
{
  //for(size_t i = 0; i <)
}

