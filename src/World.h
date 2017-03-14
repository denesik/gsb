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

// В мире существует генератор мира. Создается в конструкторе мира, уничтожается в деуструкторе.
// Мир создает сектор и применяет к нему генератор, после этого сектор добавляется в мир.
// Мир является единственным владельцем секторов.
class World
{
public:
  World(const DataBase &blocksDataBase);
  ~World();

  /// Загрузить сектор в указанной позиции.
  /// Если сектор записан на диск - загрузить с диска.
  /// В противном случае сгенерировать.
  void LoadSector(const SPos &pos);

  /// Выгрузить сектор.
  void UnLoadSector(const SPos &pos);

  const DataBase &GetBlocksDataBase() const;

  std::weak_ptr<Sector> GetSector(const SPos &pos) const;
  std::vector<std::weak_ptr<Sector>> GetColumn(const CSPos &pos) const;
  BlockId GetBlockId(const WBPos &pos) const;
  Block* GetBlockDynamic(const WBPos &pos) const;
  void CreateBlock(const WBPos &pos, BlockId id);

  UpdatableSectors &GetUpdatableSectors();

  void SetLoader(std::unique_ptr<IMapLoader> loader);
  IMapLoader & GetMaploader();

  void Wipe();

  Creature mPlayer;

private:
  std::unordered_map<SPos, std::shared_ptr<Sector>> mSectors;
  const DataBase &mBlocksDataBase;

  UpdatableSectors mUpdatableSectors;
  std::unique_ptr<IMapLoader> mLoader;
};



#endif // World_h__
