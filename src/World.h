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

// Загрузка сектора - используем текущий сектор и всех его соседей.
// Если сектор не был поставлен на загружен - ставим его на загрузку.
// Выгрузка сектора - перестаем использовать текущий сектор и всех его соседей.
// Пробегаем по секторам вокруг текущего сектора. Если сектор загружен, считаем количество загруженных секторов вокруг него.
// Если оно максимальное - сектор становится неюзабельным.
// todo: Нужно вызывать событие в начале апдейта.

// Постановка на загрузку - используем текущий сектор.
// Запуск загрузки - отправляем сектор в загрузчик.
// Завершение загрузки - ставим флаг что сектор загружен, перестаем использовать сектор.
// Пробегаем по секторам вокруг текущего сектора. Если сектор загружен, считаем количество загруженных секторов вокруг него.
// Если оно максимальное - сектор становится юзабельным.
// Вызывается в начале апдейта.

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
    bool added = false; // Добавлен ли сектор в загрузчик.
    bool loaded = false; // Загружен ли сектор.
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
  /// Текущий сектор и все его соседи станут неюзабельными.
  size_t UnuseSector(const SPos &pos);

  void CacheSector(const SPos &pos, Sector::CacheState state);

};



#endif // World_h__
