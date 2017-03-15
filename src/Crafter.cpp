#include "Crafter.h"
#include "DataBase.h"
#include <Magnum\Timeline.h>




Crafter::Crafter(const Crafter &other, Accessor &input, Accessor &output)
  : m_recipe_tag(other.m_recipe_tag), 
    mInput(static_cast<AccessorItem &>(input)), mOutput(static_cast<AccessorItem &>(output))
{

}

Crafter::Crafter(Crafter &&other, Accessor &input, Accessor &output)
  : m_recipe_tag(std::move(other.m_recipe_tag)), 
    mInput(static_cast<AccessorItem &>(input)), mOutput(static_cast<AccessorItem &>(output))
{

}

Crafter::Crafter(IRecipe::Tag tag, bool fast, Accessor &input, Accessor &output)
  : m_recipe_tag(tag), 
    mInput(static_cast<AccessorItem &>(input)), mOutput(static_cast<AccessorItem &>(output))
{

}

void Crafter::Update(const Magnum::Timeline &dt, const DataBase &db)
{
  // ����������� � ������ ������ ������, �� ������� ������ ��������� ������ ��������.
  // ���� ���� ���� �� ���� ������, ������� ������.
  // ������� �� ������ ��� ����������.
  // ��������� � ������ ����������.

  if (m_current_recipe && m_runned)
  {
    // �������.
    bool enable = true;
    const auto &components = m_current_recipe->Components();

    // ���� ����� ������� ���������� � ����� ������, ��������� �� ������� �� ����������.
    if (!m_fast_components)
    for (const auto &c : components)
    {
      // ���� ��������� ������ ������ ��� ���� � ������� �� �������.
      if (c.count > mInput.ItemCount(c.id))
      {
        m_current_recipe.reset();
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
        // ��������� �����. ���� �����, ������� ����������.
        if (!m_fast_components)
        for (const auto &c : components)
        {
          mInput.RemoveItem(c.id, c.count);
        }

        const auto &results = m_current_recipe->Results();
        for (const auto &r : results)
        {
          mOutput.AddItem(r.id, r.count);
        }
        m_current_recipe.reset();
        m_runned = false;
        m_time = 0.0f;
      }
    }
  }

  // ������, ����� �� �� ��� �� ���������.
  if (!m_current_recipe)
  {
    const auto &recipes = db.GetRecipes(m_recipe_tag, mInput.Items());
    if (!recipes.empty())
    {
      m_current_recipe = recipes.front();
    }
  }
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
  // ��������� �����. ���� ����� ������� ���������� ��� �������, ������ ���.
  if (m_fast_components)
  {
    const auto &components = m_current_recipe->Components();
    for (const auto &c : components)
    {
      mInput.RemoveItem(c.id, c.count);
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
  return m_current_recipe.is_initialized();
}