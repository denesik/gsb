#include "Sector.h"
#include "World.h"
#include "TesselatorMicroBlock.h"
#include <Magnum/Timeline.h>

using namespace Magnum;

Sector::Sector(World &world, const SPos &pos)
  : mWorld(world), mPos(pos), mChunk(*this)
{
  // generate sector
  mStaticBlocks.fill(0);
}


Sector::~Sector()
{
}

bool Sector::NeedCompile() const
{
  return mNeedCompile && 
    !mSectorAround.sectors[SectorAround::Middle].expired() &&
    !mSectorAround.sectors[SectorAround::South].expired() &&
    //!mSectorAround.sectors[SectorAround::Down].expired() &&
    !mSectorAround.sectors[SectorAround::East].expired() &&
    !mSectorAround.sectors[SectorAround::West].expired() &&
    //!mSectorAround.sectors[SectorAround::Top].expired() &&
    !mSectorAround.sectors[SectorAround::North].expired();
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
  for (size_t i = 0; i < mSectorAround.sectors.size(); ++i)
  {
    if (mSectorAround.sectors[i].expired())
    {
      mSectorAround.sectors[i] = mWorld.GetSector(mPos + SectorAround::pos[i]);
    }
  }
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
