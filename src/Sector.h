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
#include <Magnum/AbstractShaderProgram.h>
#include "ChunkHelper.h"


#include "Sector_generated.h"

class IMapGenerator;
class World;


/// �� ��������� ��������� ������ ������.
class Sector
{
public:
  friend class MapSaverToDisk;

  enum CacheState
  {
    CACHE_LOAD,
    CACHE_UNLOAD,
  };

  Sector(World &morld, const SPos &pos);
  ~Sector();

  bool NeedCompile() const;

  void NeedCompile(bool value);

  void SetCompilerData(SectorCompiler &sectorCompiler);

  void Update();

  void Draw(const Magnum::Matrix4 &vp, Magnum::AbstractShaderProgram& shader);

  SPos GetPos() const;

  World &GetWorld();

  BlockId GetBlockId(SBPos pos) const;
  Block* GetBlockDynamic(const WBPos& pos) const;

//   std::unique_ptr<TesselatorData> &GetTesselatorData(SBPos pos);
//   const std::unique_ptr<TesselatorData> &GetTesselatorData(SBPos pos) const;

  void CreateBlock(SBPos pos, BlockId id);

  std::optional<Tesselator::Data &> GetTesselatorData(SBPos pos);

  inline BlockId GetBlockId(IndexType pos) const
  {
    return mStaticBlocks[pos];
  }

  inline const std::unique_ptr<Tesselator::Data> &GetTesselatorData(IndexType pos) const
  {
    return mTesselatorData[pos];
  }

  inline const SectorAround &GetSectorAround() const
  {
    return mSectorAround;
  }

//   // ���������� ������� �������� �������.
//   void LoadSector(Sector &sector);
// 
//   // ���������� ������� �������� �������.
//   void UnloadSector(Sector &sector);

  void Cache(Sector &sector, CacheState state);

  void FlatbufferLoad(const gsb_flat::Sector &sec);

private:
  std::array<BlockId, gSectorCapacity> mStaticBlocks;
  std::array<std::unique_ptr<Block>, gSectorCapacity> mDinamicBlocks;
  std::array<std::unique_ptr<Tesselator::Data>, gSectorCapacity> mTesselatorData;
  
  const SPos mPos;

  World &mWorld;

  bool mNeedCompile = true;

  SectorAround mSectorAround;
  Chunk mChunk;
};



#endif // Sector_h__
