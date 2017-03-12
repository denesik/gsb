#pragma once
#ifndef Crafter_h__
#define Crafter_h__

#include <Magnum/Magnum.h>
#include "IRecipe.h"
#include "Timer.h"
#include "accessors/AccessorItem.h"
#include <memory>

class DataBase;

// TODO: Сделать для этого класс интерфейс.
// TODO: Написать кнструкторы
class Crafter
{
public:
  Crafter() = delete;

  Crafter(const DataBase & db, const rapidjson::Value &json);

  Crafter(const Crafter &other);

  void Update(const Magnum::Timeline &dt, const DataBase &db);
  void SetInput(Accessor &accessor);
  void SetOutput(Accessor &accessor);

  float Progress() const; // TODO int?

  // Запустить крафт.
  void Run();

  // Остановить крафт.
  void Stop();

  // Запущен крафт?
  bool Runned() const;

  // Можем запустить крафт?
  bool Ready() const;

private:
  AccessorItem *mInput;
  AccessorItem *mOutput;

  // TODO_Recipe 
  // Теги избавят от этой гадости.
  std::unique_ptr<IRecipe> m_recipe_type;

  const IRecipe *m_current_recipe = nullptr;
  bool m_runned = false;

  float m_time = 0.0f;

  // Компоненты уничтожаются в начале крафта или в конце?
  bool m_fast_components = false;

private:
  std::unique_ptr<IRecipe> LoadRecipe(const DataBase & db, const rapidjson::Value &json) const;
};



#endif // Crafter_h__
