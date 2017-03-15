#include "AccessorItem.h"
#include "../Serialize.h"
#include <Magnum/Timeline.h>
#include "../Block.h"
#include "../DataBase.h"
#include "../Item.h"
#include <algorithm>
#include "../../imgui/imgui.h"
#include "../imgui/imgui_widgets.h"
#include <string>
#include <algorithm>




AccessorItem::AccessorItem(const AccessorItem &other)
  : ParentType(other)
{}

AccessorItem::AccessorItem(AccessorItem &&other)
  : ParentType(std::move(other))
{}

AccessorItem::AccessorItem(const AccessorItem &other, Block &parent)
  : ParentType(other, parent)
{}

AccessorItem::AccessorItem(AccessorItem &&other, Block &parent)
  : ParentType(std::move(other), parent)
{}

AccessorItem::AccessorItem(const DataBase &db, const rapidjson::Value &val, Block &parent)
  : ParentType(db, val, parent)
{}

void AccessorItem::DrawGui(const Magnum::Timeline &dt)
{
  ImGui::BeginGroup();

  if (ImGui::Button("add"))
  {
    AddItem(1, 1);
  }
  ImGui::SameLine();
  if (ImGui::Button("del"))
  {
    RemoveItem(1, 1);
  }

  const auto &db = mParent.GetDataBase();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  for (size_t i = 0; i < 4; i++)
  {
    ImVec2 base_pos = ImGui::GetCursorScreenPos();
    //ImGui::PushID(i);
    if (i < mItems.size())
    {
      const auto &coord = static_cast<const Item &>(*(db.GetItem(std::get<0>(mItems[i])))).TextureCoord();
      ImGui::Image(ImTextureID(1), std::to_string(std::get<1>(mItems[i])).c_str(), ImVec2(32, 32), ImVec2(coord.left(), coord.bottom()), ImVec2(coord.right(), coord.top()));
    }
    else
    {
      ImGui::Dummy(ImVec2(32, 32));
    }
    draw_list->AddRect(base_pos, ImVec2(base_pos.x + 32, base_pos.y + 32), IM_COL32(255, 255, 255, 100));
    if ((i % 2) < 1) ImGui::SameLine();
    //ImGui::PopID();
  }
  ImGui::EndGroup();
}

size_t AccessorItem::AddItem(ItemId id, size_t count)
{
  if (auto index = find_item(id))
  {
    std::get<1>(mItems[*index]) += count;
  }
  else
  {
    mItems.emplace_back(id, count);
  }

  return count;
}

size_t AccessorItem::RemoveItem(ItemId id, size_t count)
{
  if (auto index = find_item(id))
  {
    if (count <= std::get<1>(mItems[*index])) std::get<1>(mItems[*index]) -= count;

    if (std::get<1>(mItems[*index]) == 0)
    {
      std::swap(mItems[*index], mItems.back());
      mItems.resize(mItems.size() - 1);
    }
  }

  return count;
}

size_t AccessorItem::ItemCount(ItemId id) const
{
  if (auto index = find_item(id))
  {
    return std::get<1>(mItems[*index]);
  }
  return 0;
}

const AccessorItem::ItemList & AccessorItem::Items() const
{
  return mItems;
}

boost::optional<size_t> AccessorItem::find_item(ItemId id) const
{
  auto it = std::find_if(mItems.begin(), mItems.end(), [id](const decltype(mItems)::value_type &val)
  {
    return std::get<0>(val) == id;
  });

  if (it != mItems.end())
    return (std::distance(mItems.begin(), it));

  return{};
}

