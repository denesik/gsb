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

  /// Отложенная загрузка сектора в указанной позиции.
  void LoadSector(const SPos &pos);

  /// Выгрузить сектор.
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
  // Событие вызывается после добавления сектора на карту.
  void LoadSectorEvent(Sector &sector);
  // Событие вызывается перед удалением сектора с карты.
  void UnloadSectorEvent(Sector &sector);

  /// Сообщить миру что мы хотим использовать этот сектор.
  /// Если сектор отсутствует, в мир добавится пустой незагруженный сектор.
  size_t UseSector(const SPos &pos);

  /// Сообщить миру что мы перестали использовать этот сектор.
  size_t UnuseSector(const SPos &pos);

};



#endif // World_h__
