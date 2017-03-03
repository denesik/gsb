#pragma once
#ifndef World_h__
#define World_h__

#include "tools\CoordSystem.h"
#include <unordered_map>
#include <memory>
#include "Sector.h"
#include "UpdatableSectors.h"

class World
{
public:
  World(const BlocksDataBase &blocksDataBase);
  ~World();

  /// ��������� ������ � ��������� �������.
  /// ���� ������ ������� �� ���� - ��������� ������.
  /// � ��������� ������ �������������.
  void LoadSector(const SPos &pos);

  /// ��������� ������.
  void UnLoadSector(const SPos &pos);

  const BlocksDataBase &GetBlocksDataBase() const;

  std::shared_ptr<Sector> GetSector(const SPos &pos);

  UpdatableSectors &GetUpdatableSectors();
private:
  std::unordered_map<SPos, std::shared_ptr<Sector>> mSectors;
  const BlocksDataBase &mBlocksDataBase;

  UpdatableSectors mUpdatableSectors;
};



#endif // World_h__
