#include "Crafter.h"
#include "DataBase.h"
#include "RecipeHand.h"
#include <Magnum\Timeline.h>



//TODO_Recipe 
// Теги избавят от этой гадости ввиде клонирования.
Crafter::Crafter(const Crafter &other)
  : m_recipe_type(other.m_recipe_type->Clone()), m_fast_components(other.m_fast_components)
{

}

Crafter::Crafter(const std::unique_ptr<IRecipe> &recipe, bool fast, AccessorItem *input, AccessorItem *output)
  : m_recipe_type(recipe->Clone()), m_fast_components(fast), mInput(input), mOutput(output)
{

}


void Crafter::Update(const Magnum::Timeline &dt, const DataBase &db)
{
  // Запрашиваем у инпута список итемов, по данному списку формируем список рецептов.
  // Если есть хотя бы один рецепт, крафтим первый.
  // Удаляем из инпута все компоненты.
  // Добавляем в оутпут результаты.

  auto val = CrafterFactory::Get().Create("Crafter", m_recipe_type->Clone(), true, mInput, mOutput);
  int t = 0;


  if (m_current_recipe && m_runned)
  {
    // Крафтим.
    bool enable = true;
    const auto &components = m_current_recipe->Components();

    // Если нужно удалить компоненты в конце крафта, проверяем не исчезли ли компоненты.
    if (!m_fast_components)
    for (const auto &c : components)
    {
      // Если требуется больше итемов чем есть в сундуке не крафтим.
      if (c.count > mInput->ItemCount(c.id))
      {
        m_current_recipe = nullptr;
        m_runned = false;
        m_time = 0.0f;
        enable = false;
        break;
      }
    }

    if (enable)
    {
      m_time += dt.previousFrameDuration();

      if (m_time >= m_current_recipe->Time())
      {
        // Закончили крафт. Если нужно, удаляем компоненты.
        if (!m_fast_components)
        for (const auto &c : components)
        {
          mInput->RemoveItem(c.id, c.count);
        }

        const auto &results = m_current_recipe->Results();
        for (const auto &r : results)
        {
          mOutput->AddItem(r.id, r.count);
        }
        m_current_recipe = nullptr;
        m_runned = false;
        m_time = 0.0f;
      }
    }
  }

  // Узнаем, можем ли мы что то скрафтить.
  if (!m_current_recipe)
  {
    const auto &recipes = db.GetRecipes(*m_recipe_type, mInput->Items());
    if (!recipes.empty())
    {
      m_current_recipe = recipes.front();
    }
  }
}



void Crafter::SetInput(Accessor &accessor)
{
  mInput = &static_cast<AccessorItem &>(accessor);
}

void Crafter::SetOutput(Accessor &accessor)
{
  mOutput = &static_cast<AccessorItem &>(accessor);
}

float Crafter::Progress() const
{
  if (m_current_recipe)
  {
    return m_time / m_current_recipe->Time();
  }

  return 0.0f;
}

void Crafter::Run()
{
  // Запускаем крафт. Если нужно удалить компоненты при запуске, делаем это.
  if (m_fast_components)
  {
    const auto &components = m_current_recipe->Components();
    for (const auto &c : components)
    {
      mInput->RemoveItem(c.id, c.count);
    }
  }
  m_runned = true;
}

void Crafter::Stop()
{
  m_runned = false;
}

bool Crafter::Runned() const
{
  return Ready() && m_runned;
}

bool Crafter::Ready() const
{
  return m_current_recipe != nullptr;
}