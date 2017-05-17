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

// Событие Load вызывается после загрузки сектора.
// После этого с сектором можно делать что угодно.
// Внутри сектора будут храниться ссылки на все соседние сектора.
// Все соседние сектора также будут загружены и с ними можно делать что угодно.
// Событие Unload вызывется перед выгрузкой сектора или какого то из его соседних секторов.
// По завершению данного события нельзя пользоваться выгруженным сектором.

class World
{
public:
  World(const DataBase &blocksDataBase);
  ~World();

  /// Отложенная загрузка сектора в указанной позиции.
  /// Создается сектор и все соседи вокруг него.
  /// Данные сектора ставятся на загрузку.
  // Событие загрузки сектора вызовется когда будут загружены указанный сектор и все его соседи.
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
    bool added = false; // Добавлен ли сектор в загрузчик.
    bool loaded = false; // Загружен ли сектор.
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
  /// Сообщить миру что мы хотим использовать этот сектор.
  /// Если сектор отсутствует, в мир добавится пустой незагруженный сектор.
  void UseSector(const SPos &pos);

  /// Сообщить миру что мы перестали использовать этот сектор.
  /// Текущий сектор и все его соседи станут неюзабельными.
  void UnuseSector(const SPos &pos);

  size_t CountLoaded(const SPos &pos);

  void CacheSector(const SPos &pos, Sector::CacheState state);

  bool OnSectorLoadBegin(Worker &worker, Sector &sector);
  bool OnSectorLoadEnd(Worker &worker, Sector &sector);

  std::vector<SPos> mUnloadSectors;

};



#endif // World_h__
