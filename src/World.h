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
