#include "BlockAutoCrafter.h"
#include "agent\agents\Chest.h"
#include "Recipe.h"
#include "BlocksDataBase.h"
#include "..\imgui\imgui.h"



BlockAutoCrafter::BlockAutoCrafter()
{
}


BlockAutoCrafter::~BlockAutoCrafter()
{
}

BlockAutoCrafter::BlockAutoCrafter(const BlockAutoCrafter &other)
  : BlockDynamicPart(other)
{

}

std::unique_ptr<BlockDynamicPart> BlockAutoCrafter::Clone()
{
  BlockDynamicPart *ptr = new BlockAutoCrafter(*this);
  std::unique_ptr<BlockDynamicPart> obj;
  obj.reset(ptr);
  return std::move(obj);
}

void BlockAutoCrafter::DrawGui(const Magnum::Timeline &dt)
{
  //BlockDynamicPart::DrawGui(dt);

//   for (size_t i = 0; i < mAgents.size(); ++i)
//   {
//     ImGui::PushID(i);
//     mAgents[i]->DrawGui(dt);
//     ImGui::PopID();
//   }

  float progress = 0.0f;
  if (m_current_recipe)
  {
    progress = ((mTimer.Elapsed() * 100.0f) / m_current_recipe->Time()) / 100.0f;
  }

  // time - 100
  // elap - x
  // x = elap * 100 / time
  if (progress > 50.0f)
  {
    int t = 0;
  }

  auto &input = static_cast<Chest &>(*mAgents[0]);
  auto &output = static_cast<Chest &>(*mAgents[1]);
  ImGui::PushID(0);
  input.DrawGui(dt);
  ImGui::PopID();

  ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));

  ImGui::PushID(1);
  output.DrawGui(dt);
  ImGui::PopID();

  Update(dt);
}

void BlockAutoCrafter::Update(const Magnum::Timeline &dt)
{
  // Запрашиваем у инпута список итемов, по данному списку формируем список рецептов.
  // Если есть хотя бы один рецепт, крафтим первый.
  // Удаляем из инпута все компоненты.
  // Добавляем в оутпут результаты.
  auto &input = static_cast<Chest &>(*mAgents[0]);
  auto &output = static_cast<Chest &>(*mAgents[1]);
  auto &db = *mDb;

  if (m_current_recipe)
  {
    if (mTimer.Elapsed() >= m_current_recipe->Time())
    {
      const auto &components = m_current_recipe->Components();
      const auto &results = m_current_recipe->Results();
      for (const auto &c : components)
      {
        input.RemoveItem(c.id, c.count);
      }
      for (const auto &r : results)
      {
        output.AddItem(r.id, r.count);
      }
      m_current_recipe = nullptr;
    }
  }
  
  if (!m_current_recipe)
  {
    const auto &recipes = db.GetRecipes(Recipe(), input.Items());
    if (!recipes.empty())
    {
      m_current_recipe = recipes.front();
    }
    mTimer.Start();
  }
}
