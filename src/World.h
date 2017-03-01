#pragma once
#ifndef World_h__
#define World_h__

#include "tools\CoordSystem.h"
#include <unordered_map>
#include <memory>
#include "Sector.h"

class World
{
public:
  World(const BlocksDataBase &blocksDataBase);
  ~World();

  /// Загрузить сектор в указанной позиции.
  /// Если сектор записан на диск - загрузить сдиска.
  /// В противном случае сгенерировать.
  void LoadSector(const SPos &pos);

  /// Выгрузить сектор.
  void UnLoadSector(const SPos &pos);

  const BlocksDataBase &GetBlocksDataBase() const;

  std::shared_ptr<Sector> GetSector(const SPos &pos);

private:
  std::unordered_map<SPos, std::shared_ptr<Sector>> mSectors;

  const BlocksDataBase &mBlocksDataBase;
};



#endif // World_h__
