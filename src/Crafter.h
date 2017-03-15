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
  virtual ~Crafter() = default;

  Crafter(const Crafter &other) = delete;
  Crafter(Crafter &&other) = delete;
  /// Не используем операторы копирования и перемещения.
  const Crafter &operator=(const Crafter &other) = delete;
  Crafter &operator=(Crafter &&other) = delete;

  Crafter(const Crafter &other, Accessor &input, Accessor &output);
  Crafter(Crafter &&other, Accessor &input, Accessor &output);

  /// Создаем элемент через фабрику.
  Crafter(IRecipe::Tag tag, bool fast, Accessor &input, Accessor &output);

  void Update(const Magnum::Timeline &dt, const DataBase &db);

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
  AccessorItem &mInput;
  AccessorItem &mOutput;

  IRecipe::Tag m_recipe_tag;

  boost::optional<const IRecipe &> m_current_recipe;
  bool m_runned = false;

  float m_time = 0.0f;

  // Компоненты уничтожаются в начале крафта или в конце?
  bool m_fast_components = false;

private:
};


#endif // Crafter_h__
