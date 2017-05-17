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
#include <type_traits>


class MapLoader;

// ������� Load ���������� ����� �������� �������.
// ����� ����� � �������� ����� ������ ��� ������.
// ������ ������� ����� ��������� ������ �� ��� �������� �������.
// ��� �������� ������� ����� ����� ��������� � � ���� ����� ������ ��� ������.
// ������� Unload ��������� ����� ��������� ������� ��� ������ �� �� ��� �������� ��������.
// �� ���������� ������� ������� ������ ������������ ����������� ��������.

class World
{
public:
  World(const DataBase &blocksDataBase);
  ~World();

  /// ���������� �������� ������� � ��������� �������.
  /// ��������� ������ � ��� ������ ������ ����.
  /// ������ ������� �������� �� ��������.
  // ������� �������� ������� ��������� ����� ����� ��������� ��������� ������ � ��� ��� ������.
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
    bool added = false; // �������� �� ������ � ���������.
    bool loaded = false; // �������� �� ������.
  };
  const DataBase &mBlocksDataBase;

  std::unique_ptr<IMapGenerator> mWorldGenerator;

  UpdatableSectors mUpdatableSectors;

  std::unordered_map<SPos, SectorElement> mSectors;

  WorldSectorObserver mWorldSectorObserver;

  Sector mFakeSector;

  struct Worker
  {
    Worker(IMapGenerator &generator)
      : generator(generator)
    {

    }

    void Process(std::reference_wrapper<Sector> sector)
    {
      generator.Generate(sector.get());
    }

    IMapGenerator &generator;
  };

  ThreadProcess<Worker, std::reference_wrapper<Sector>> mSectorLoader;

private:
  /// �������� ���� ��� �� ����� ������������ ���� ������.
  /// ���� ������ �����������, � ��� ��������� ������ ������������� ������.
  void UseSector(const SPos &pos);

  /// �������� ���� ��� �� ��������� ������������ ���� ������.
  /// ������� ������ � ��� ��� ������ ������ �������������.
  void UnuseSector(const SPos &pos);

  size_t CountLoaded(const SPos &pos);

  void CacheSector(const SPos &pos, Sector::CacheState state);

  bool OnSectorLoadBegin(Worker &worker, Sector &sector);
  bool OnSectorLoadEnd(Worker &worker, Sector &sector);

  std::vector<SPos> mUnloadSectors;

};



#endif // World_h__
