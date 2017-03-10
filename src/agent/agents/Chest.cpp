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

Chest::Chest(const Chest& other)
{
  AddItem(1, 1);
}

void Chest::JsonLoad(BlocksDataBase & db, const rapidjson::Value& val)
{
  Agent::JsonLoad(db, val);
}

void Chest::DrawGui(const Magnum::Timeline &dt)
{
  if (ImGui::Button("add"))
  {
    AddItem(1, 1);
  }

  const auto &db = mParent->GetDataBase();
  for (size_t i = 0; i < 16; i++)
  {
    //ImGui::PushID(i);
    if (i < mItems.size())
    {
      static_cast<const Item &>(*(db.GetItem(std::get<0>(mItems[i])))).DrawGui(dt);
    }
    else
    {
      ImGui::Dummy(ImVec2(32, 32));
    }
    if ((i % 4) < 3) ImGui::SameLine();
    //ImGui::PopID();
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

const Chest::ItemList & Chest::Items() const
{
  return mItems;
}

// void Chest::Test()
// {
//   // Тестовая функция.
//   // Имитируем выполнение из блока.
//   BlockDynamicPart *block = nullptr;
//   for (int i = 0; i < 6; ++i)
//   {
//     block = mParent->GetNeighbour(static_cast<SideIndex>(i));
//     if (block != nullptr)
//       break;
//   }
// 
//   if (block != nullptr)
//   for (int i = 0; i < 6; ++i)
//   {
//     auto agent = block->GetAgent(Id(), static_cast<SideIndex>(i), AgentDirection::in);
//     if (agent)
//     {
//       auto &a = static_cast<Chest &>(*agent);
//       if (!mItems.empty())
//       {
//         a.AddItem(std::get<0>(mItems.back()), std::get<1>(mItems.back()));
//         mItems.resize(mItems.size() - 1);
//       }
//     }
//   }
// 
// }
