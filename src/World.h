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

class MapLoader;

class TaskGenerate
{
public:
  TaskGenerate(World &morld, const SPos &pos);

  bool Begin(MapLoader &loader);

  void End(const MapLoader &loader);

private:
  std::shared_ptr<Sector> mSector;
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

  UpdatableSectors &GetUpdatableSectors();
  
  IMapGenerator &GetWorldGenerator();

  void Wipe();

  Creature mPlayer;

  void Update();

private:
  const DataBase &mBlocksDataBase;

  friend class TaskGenerate;
  std::unique_ptr<IMapGenerator> mWorldGenerator;
  ThreadWorker<TaskGenerate, MapLoader> mLoaderWorker;

  UpdatableSectors mUpdatableSectors;

  std::unordered_map<SPos, std::shared_ptr<Sector>> mSectors;
  std::unordered_set<SPos> mLoadedSectors;
};



#endif // World_h__
