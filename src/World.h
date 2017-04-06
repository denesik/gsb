#pragma once
#ifndef World_h__
#define World_h__

#include "tools\CoordSystem.h"
#include <unordered_map>
#include <memory>
#include "Sector.h"
#include "UpdatableSectors.h"
#include "IMapGenerator.h"
#include "IMapLoader.h"
#include "Creature.h"
#include "ThreadWorker.h"
#include <unordered_set>
#include "..\WorldSectorObserver.h"
#include <boost\optional\optional.hpp>


class MapLoader;

// class TaskGenerate
// {
// public:
//   TaskGenerate(World &morld, const SPos &pos);
// 
//   bool Begin(MapLoader &loader);
// 
//   void End(const MapLoader &loader);
// };



class World
{
public:
  World(const DataBase &blocksDataBase);
  ~World();

  /// ���������� �������� ������� � ��������� �������.
  void LoadSector(const SPos &pos);

  /// ��������� ������.
  void UnLoadSector(const SPos &pos);


  const DataBase &GetBlocksDataBase() const;

  //boost::optional<Sector> GetSector(const SPos &pos) const;

  BlockId GetBlockId(const WBPos &pos) const;
  Block* GetBlockDynamic(const WBPos &pos) const;
  void CreateBlock(const WBPos &pos, BlockId id);

  UpdatableSectors &GetUpdatableSectors();
  
  IMapGenerator &GetWorldGenerator();

  void Wipe();

  Creature mPlayer;

  void Update();

  inline WorldSectorObserver &GetWorldSectorObserver()
  {
    return mWorldSectorObserver;
  }

  inline Sector &FakeSector()
  {
    return mFakeSector;
  }

private:
  struct SectorElement
  {
    template<class ...Args>
    SectorElement(Args &&...args)
      : sector(std::forward<Args>(args)...)
    {}
    Sector sector;
    size_t count = 0;
  };
  const DataBase &mBlocksDataBase;

  std::unique_ptr<IMapGenerator> mWorldGenerator;

  UpdatableSectors mUpdatableSectors;

  std::unordered_map<SPos, SectorElement> mSectors;

  WorldSectorObserver mWorldSectorObserver;

  Sector mFakeSector;

private:
  // ������� ���������� ����� ���������� ������� �� �����.
  void LoadSectorEvent(Sector &sector);
  // ������� ���������� ����� ��������� ������� � �����.
  void UnloadSectorEvent(Sector &sector);

  /// �������� ���� ��� �� ����� ������������ ���� ������.
  /// ���� ������ �����������, � ��� ��������� ������ ������������� ������.
  size_t UseSector(const SPos &pos);

  /// �������� ���� ��� �� ��������� ������������ ���� ������.
  size_t UnuseSector(const SPos &pos);

};



#endif // World_h__
