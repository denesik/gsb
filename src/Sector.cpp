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
  mChunk.SetCompilerData(sectorCompiler);
}

void Sector::Update()
{
  //  ешируем соседние сектора.
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

void Sector::LoadSector(Sector &sector)
{
  auto pos = sector.GetPos() - mPos;
  if (mSectorAround.inside(pos))
  {
    mSectorAround.sectors[mSectorAround.to_index(pos)] = sector;
    if (m_is_loaded && sector.is_loaded())
      change_count_loaded(1);
  }
}

void Sector::UnloadSector(Sector &sector)
{
  auto pos = sector.GetPos() - mPos;
  if (mSectorAround.inside(pos))
  {
    mSectorAround.sectors[mSectorAround.to_index(pos)] = mWorld.FakeSector();
    if (m_is_loaded && sector.is_loaded())
      change_count_loaded(-1);
  }
}

void Sector::set_loaded()
{
  // «агрузили сектор данными.
  // —читаем количество загруженных секторов вокруг.
  // —ообщаем соседним загруженным секторам что текущий сектор загрузилс€.

  m_is_loaded = true;
  int count_load = 0;
  for (auto &s : mSectorAround.sectors)
  {
    auto &sector = static_cast<Sector &>(s);
    if (sector.is_loaded())
    {
      ++count_load;
      sector.change_count_loaded(1);
    }
  }
  change_count_loaded(count_load);
}

void Sector::change_count_loaded(int count)
{
  const auto max_count = mSectorAround.sectors.size() - 1;
  auto new_count = m_count_loaded += count;
  assert(count != 0 && new_count >= 0 && new_count <= max_count);

  if (m_count_loaded == max_count)
  {
    // —ектор перестал быть готовым к обработке.
  }

  if (new_count == max_count)
  {
    // —ектор готов к обработке.
  }

  m_count_loaded = new_count;
}
