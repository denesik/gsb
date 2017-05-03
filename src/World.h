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


// �������� ������� � �����: 
// ���� ���� ������ �������.
// ������ id ������� �����.
// ������� ���� � ��������� �� ����� ��, ���������� ������ ��� ����� ���������� � ������� �������� �����.

// �������� ��: 
// �� ���� ���� ����������� �����, ���� ������� - ��������� �������������� ������.
// ���� �� �������, �������������� ������ ��������� �������������.
// ���� �� ����� ��� � ����� �����������, ���� ����������� �����������.

// � ���� ���������� ��������� ����. ��������� � ������������ ����, ������������ � ������������.
// ��� ������� ������ � ��������� � ���� ���������, ����� ����� ������ ����������� � ���.
// ��� �������� ������������ ���������� ��������.
// �������� ��������:
// ������ ����� ����������� � ����� ��� ��������� �����������.

// ��������� ������ � ������ ��������.
// ��������� �� ������ ��������, ��������� ���� �� ���������, ���� ����, ��������� �������� �� ������.
// ���� ������ ��������, ���������� ��� � �����, ����������� ���������.
// ���� ��������� �����������, ���� ��������������� ��������� � ���������, ���� ������� ������.

// ���������� �������� ��������.
// ���� ������ ����������,�� ����� �� �����������.
// ���� �� ����������, ���������� �������� ������� � ��������� ������.

class MapLoaderFromGenerator;

class TaskLoad
{
public:
  TaskLoad(World &morld, const SPos &pos);

  bool Begin(IMapLoader &loader);

  void End(const IMapLoader &loader);

private:
  std::shared_ptr<Sector> mSector;
};

class TaskUnload
{
public:
  TaskUnload(Sector * sector); //TODO: refs

  bool Begin(IMapSaver &loader);

  void End(const IMapSaver &loader);

private:
  Sector * mSector;
};

class UnloadObserver : public IWorldSectorEvent
{
public:
  UnloadObserver(IThreadWorker<TaskUnload> &saverWorker);

  // ������������ ����� IWorldSectorEvent
  void Load(Sector &sector) override;
  void UnLoad(Sector &sector) override;

private:
  IThreadWorker<TaskUnload> &mSaverWorker;
};

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

  std::weak_ptr<Sector> GetSector(const SPos &pos) const;

  BlockId GetBlockId(const WBPos &pos) const;
  Block* GetBlockDynamic(const WBPos &pos) const;
  void CreateBlock(const WBPos &pos, BlockId id);

  void DestroyBlock(const WBPos &pos);

  UpdatableSectors &GetUpdatableSectors();
  
  IMapGenerator &GetWorldGenerator();

  void Wipe();

  Creature mPlayer;

  void Update();

  inline WorldSectorObserver &GetWorldSectorObserver()
  {
    return mWorldSectorObserver;
  }

private:
  const DataBase &mBlocksDataBase;

  friend class TaskLoad;
  friend class TaskUnload;
  std::unique_ptr<IMapGenerator> mWorldGenerator;
  std::unique_ptr<IThreadWorker<TaskLoad>> mLoaderWorker;

  std::unique_ptr<UnloadObserver> unloadObserver;
  std::unique_ptr<IThreadWorker<TaskUnload>> mSaverWorker;

  UpdatableSectors mUpdatableSectors;

  std::unordered_map<SPos, std::shared_ptr<Sector>> mSectors;
  std::unordered_set<SPos> mLoadedSectors;

  WorldSectorObserver mWorldSectorObserver;

private:
  // ������� ���������� ����� ���������� ������� �� �����.
  void LoadSectorEvent(Sector &sector);
  // ������� ���������� ����� ��������� ������� � �����.
  void UnloadSectorEvent(Sector &sector);

};



#endif // World_h__
