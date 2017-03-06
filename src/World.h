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

  std::weak_ptr<Sector> GetSector(const SPos &pos) const;
  std::vector<std::weak_ptr<Sector>> GetColumn(const CSPos &pos) const;
  BlockId GetBlockId(const WBPos &pos) const;
  void CreateBlock(const WBPos &pos, BlockId id);

  UpdatableSectors &GetUpdatableSectors();

  void SetLoader(std::unique_ptr<IMapLoader> loader);

private:
  std::unordered_map<SPos, std::shared_ptr<Sector>> mSectors;
  const BlocksDataBase &mBlocksDataBase;

  UpdatableSectors mUpdatableSectors;
  std::unique_ptr<IMapLoader> mLoader;
};



#endif // World_h__
