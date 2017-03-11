#include "Crafter.h"
#include "BlocksDataBase.h"
#include "Recipe.h"
#include <Magnum\Timeline.h>

Crafter::Crafter(std::unique_ptr<IRecipe> recipe)
  : m_recipe_type(std::move(recipe))
{
}

void Crafter::Update(const Magnum::Timeline &dt, const BlocksDataBase &db)
{
  // Запрашиваем у инпута список итемов, по данному списку формируем список рецептов.
  // Если есть хотя бы один рецепт, крафтим первый.
  // Удаляем из инпута все компоненты.
  // Добавляем в оутпут результаты.

  if (m_current_recipe && m_runned)
  {
    // Крафтим.
    m_time += dt.previousFrameDuration();

    if (m_time >= m_current_recipe->Time())
    {
      const auto &components = m_current_recipe->Components();
      const auto &results = m_current_recipe->Results();
      for (const auto &c : components)
      {
        mInput->RemoveItem(c.id, c.count);
      }
      for (const auto &r : results)
      {
        mOutput->AddItem(r.id, r.count);
      }
      m_current_recipe = nullptr;
      m_runned = false;
      m_time = 0.0f;
    }
  }

  if (!m_current_recipe)
  {
    const auto &recipes = db.GetRecipes(*m_recipe_type, mInput->Items());
    if (!recipes.empty())
    {
      m_current_recipe = recipes.front();
    }
  }
}

void Crafter::SetInput(Agent &agent)
{
  mInput = &static_cast<Chest &>(agent);
}

void Crafter::SetOutput(Agent &agent)
{
  mOutput = &static_cast<Chest &>(agent);
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
