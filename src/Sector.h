#pragma once
#ifndef Sector_h__
#define Sector_h__

#include <Magnum/Magnum.h>

#include "StaticBlock.h"
#include <array>
#include "SectorCompiler.h"
#include "tools\CoordSystem.h"
#include <memory>
#include "Block.h"
#include <MagnumExternal/Optional/optional.hpp>
#include "Tesselator.h"

class IMapGenerator;
class World;

/// ѕо умолчанию создаетс€ пустой сектор.
class Sector
{
public:
  Sector(World &morld, const SPos &pos);
  ~Sector();

  bool NeedCompile() const;

  void NeedCompile(bool value);

  void SetCompilerData(SectorCompiler &sectorCompiler);

  void Update();

  SPos GetPos() const;

  void ApplyGenerator(IMapGenerator &generator);

  BlockId GetBlockId(SBPos pos) const;
  Block* GetBlockDynamic(const WBPos& pos) const;

//   std::unique_ptr<TesselatorData> &GetTesselatorData(SBPos pos);
//   const std::unique_ptr<TesselatorData> &GetTesselatorData(SBPos pos) const;

  void CreateBlock(SBPos pos, BlockId id);

  std::optional<TesselatorData &> GetTesselatorData(SBPos pos);

private:
  std::array<BlockId, SECTOR_CAPACITY> mStaticBlocks;
  std::array<std::unique_ptr<Block>, SECTOR_CAPACITY> mDinamicBlocks;
  std::array<std::unique_ptr<TesselatorData>, SECTOR_CAPACITY> mTesselatorData;
  
  const SPos mPos;

  World &mWorld;

  bool mNeedCompile = true;
};



#endif // Sector_h__
