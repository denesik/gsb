#pragma once
#ifndef Sector_h__
#define Sector_h__

#include <Magnum/Magnum.h>

#include "BlockStaticPart.h"
#include <array>
#include "SectorCompiler.h"
#include "tools\CoordSystem.h"
#include <memory>
#include "BlockDynamicPart.h"
#include <MagnumExternal/Optional/optional.hpp>

class IMapGenerator;
class World;

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
  BlockDynamicPart* GetBlockDynamic(const WBPos& pos) const;

  void CreateBlock(SBPos pos, BlockId id);

  std::optional<TesselatorData &> GetTesselatorData(SBPos pos);

private:
  std::array<BlockId, SECTOR_CAPACITY> mStaticBlocks;
  std::array<std::unique_ptr<BlockDynamicPart>, SECTOR_CAPACITY> mDinamicBlocks;
  const SPos mPos;

  World &mWorld;

  bool mNeedCompile = true;
};



#endif // Sector_h__
