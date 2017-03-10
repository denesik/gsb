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
  BlockDynamicPart::DrawGui(dt);
  if (ImGui::Button("Update"))
  {
    Update();
  }
}

void BlockAutoCrafter::Update()
{
  // Запрашиваем у инпута список итемов, по данному списку формируем список рецептов.
  // Если есть хотя бы один рецепт, крафтим первый.
  // Удаляем из инпута все компоненты.
  // Добавляем в оутпут результаты.
  auto &input = static_cast<Chest &>(*mAgents[0]);
  auto &output = static_cast<Chest &>(*mAgents[1]);
  auto &db = *mDb;

  const auto &recipes = db.GetRecipes(Recipe(), input.Items());
  if (!recipes.empty())
  {
    const auto &components = recipes.front()->Components();
    const auto &results = recipes.front()->Results();
    for (auto c : components)
    {
      input.RemoveItem(c.id, c.count);
    }
    for (auto r : results)
    {
      output.AddItem(r.id, r.count);
    }
  }
}
