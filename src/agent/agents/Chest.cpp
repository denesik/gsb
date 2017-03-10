#include "Chest.h"
#include "../../Serialize.h"
#include <Magnum/Timeline.h>
#include "../../../imgui/imgui.h"
#include "../../BlockDynamicPart.h"
#include "../../BlocksDataBase.h"
#include "../../Item.h"
#include <algorithm>
#include "Recipe.h"

Chest::Chest()
{
}

Chest::Chest(const Chest& other) : test(other.test)
{
  AddItem(1, 1);
}

void Chest::JsonLoad(BlocksDataBase & db, const rapidjson::Value& val)
{
  JSONLOAD(NVP(test));
}

void Chest::DrawGui(const Magnum::Timeline &dt)
{
  ImGui::Text(std::to_string(test).c_str());
  if (ImGui::Button("add"))
  {
    test++;
    AddItem(1, 1);
  }

  const auto &db = mParent->GetDataBase();
  for (auto &i : mItems)
  {
    static_cast<const Item &>(*(db.GetItem(std::get<0>(i)))).DrawGui(dt);
  }

  if (ImGui::Button("rec")) 
  {
    const auto &recipes = db.GetRecipes(Recipe(), mItems);
    for (const auto &rec : recipes)
    {
      rec->Craft(*this);
    }
  }
}

void Chest::AddItem(ItemId id, size_t count)
{
  mItems.emplace_back(id, count);
}

bool Chest::RemoveItem(ItemId id, size_t count)
{
  auto it = std::find_if(mItems.begin(), mItems.end(), [id](const decltype(mItems)::value_type &val)
  {
    return std::get<0>(val) == id;
  });

  if (it != mItems.end())
  {
    mItems.erase(it);
    return true;
  }

  return false;
}

const std::vector<std::tuple<ItemId, size_t>> & Chest::Items() const
{
  return mItems;
}
