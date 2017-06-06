#pragma once
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
class Generator
{
public:
  Generator() = delete;
  virtual ~Generator() = default;

  Generator(const Generator &other) = delete;
  Generator(Generator &&other) = delete;
  /// Не используем операторы копирования и перемещения.
  const Generator &operator=(const Generator &other) = delete;
  Generator &operator=(Generator &&other) = delete;

  Generator(const Generator &other, Accessor &input, Accessor &output);
  Generator(Generator &&other, Accessor &input, Accessor &output);

  /// Создаем элемент через фабрику.
  Generator(IRecipe::Tag tag, bool fast, Accessor &input, Accessor &output);

  void Update(const Magnum::Timeline &dt, const DataBase &db);

  float Progress() const; // TODO int?

                          // Запустить крафт.
  void Run();

  // Остановить крафт.
  void Stop();

  // Запущен крафт?
  bool IsRunned() const;

  // Можем запустить крафт?
  bool IsReady() const;

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

