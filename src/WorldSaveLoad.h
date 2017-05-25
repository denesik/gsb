#ifndef _WORLD_SAVE_LOAD__H_
#define _WORLD_SAVE_LOAD__H_
#pragma once

#include <boost/optional.hpp>
#include <memory>

class Sector;

/// —ектор сохранаетс€ при удалении из мира.
/// ѕроисходит копирование данных.
/// ѕри загрузке, провер€етс€ нужно ли загрузить сектор из файла.
/// ѕровер€ем не сохран€етс€ ли сектор, или есть ли файл дл€ загрузки.
/// ≈сли сектор сохран€етс€, дожидаемс€ окончани€ и копируем сектор.
/// ≈сли найден файл, читаем из файла.
class WorldSaveLoad
{
public:
  /// «агрузить сектор.
  /// ¬озвращает true если сектор будет поставлен на загрузку и false в противном случае.
  bool Load(Sector &sector);

  /// —охранить сектор.
  /// ¬озвращает true если сектор будет сохранен и false в противном случае.
  /// —ектор может быть не поставлен на сохранение, если он сохран€етс€ в данный момент.
  bool Save(const Sector &sector);

  void Update();

private:
  struct Data
  {
    enum 
    {
      WAIT,
      SAVE,
      LOAD,
    } status = WAIT; //  ака€ операци€ выполн€етс€ в данный момент.
    
    boost::optional<Sector &> sector_for_load;
    std::unique_ptr<Sector> sector_for_save;
  };

};


#endif
