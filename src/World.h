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


// Загрузка сектора с диска: 
// Ищем файл данных сектора.
// Читаем id каждого блока.
// Создаем блок с указанным ид через бд, уникальные данные для блока передаются в функцию создания блока.

// Загрузка БД: 
// Бд ищет файл регистрации блока, если находит - загружает идентификаторы блоков.
// Если не находит, идентификаторы блоков генерятся автоматически.
// Если ид блока нет в файле регистрации, файл регистрации обновляется.

// В мире существует генератор мира. Создается в конструкторе мира, уничтожается в деуструкторе.
// Мир создает сектор и применяет к нему генератор, после этого сектор добавляется в мир.
// Мир является единственным владельцем секторов.
// Загрузка секторов:
// Сектор может загрузиться с диска или создаться генератором.

// Добавляем сектор в список загрузки.
// Пробегаем по списку загрузки, проверяем есть ли загрузчик, если есть, проверяем загружен ли сектор.
// Если сектор загружен, отправляем его в карту, освобождаем загрузчик.
// Если загрузчик отсутствует, ищем соответствующий загрузчик и добавляем, если таковой найден.

// Отложенная выгрузка секторов.
// Если сектор существует,он сразу же выгружается.
// Если не существует, дожидаемся загрузчи сектора и выгружаем сектор.

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

  // Унаследовано через IWorldSectorEvent
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

  /// Отложенная загрузка сектора в указанной позиции.
  void LoadSector(const SPos &pos);

  /// Выгрузить сектор.
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
  // Событие вызывается после добавления сектора на карту.
  void LoadSectorEvent(Sector &sector);
  // Событие вызывается перед удалением сектора с карты.
  void UnloadSectorEvent(Sector &sector);

};



#endif // World_h__
