#pragma once
#ifndef Crafter_h__
#define Crafter_h__

#include <Magnum/Magnum.h>
#include "IRecipe.h"
#include "Timer.h"
#include "accessors/AccessorItem.h"
#include <memory>
#include "TemplateFactory.h"

class DataBase;

// TODO: Сделать для этого класс интерфейс.
// TODO: Написать кнструкторы
// Вероятно классы логики должны настраиваться родителем (класс блока) а не через json.
// Простой крафтер. Крафтит итемы из инпут акцессера в оутпут акцессер.
class Crafter
{
public:
  Crafter() = delete;

  Crafter(IRecipe::Tag tag, bool fast, AccessorItem * a = nullptr, AccessorItem * b = nullptr);


  Crafter(bool fast, AccessorItem * input = nullptr, AccessorItem * output = nullptr)
    : m_fast_components(fast), mInput(input), mOutput(output) {};

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
  AccessorItem *mInput = nullptr;
  AccessorItem *mOutput = nullptr;

  IRecipe::Tag m_recipe_tag;

  const IRecipe *m_current_recipe = nullptr;
  bool m_runned = false;

  float m_time = 0.0f;

  // Компоненты уничтожаются в начале крафта или в конце?
  bool m_fast_components = false;

private:
};


#endif // Crafter_h__
