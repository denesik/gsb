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
#include "../../InventorySlot.h"
#include <ItemContainerContext.h>

AccessorItem::AccessorItem(const AccessorItem &other)
  : ParentType(other)
{
  mItems.resize(other.mItems.size());
}

AccessorItem::AccessorItem(AccessorItem &&other)
  : ParentType(std::move(other))
{
  mItems = std::move(other.mItems);
}

AccessorItem::AccessorItem(const AccessorItem &other, Block &parent)
  : ParentType(other, parent)
{
  mItems.resize(other.mItems.size());
}

AccessorItem::AccessorItem(AccessorItem &&other, Block &parent)
  : ParentType(std::move(other), parent)
{
  mItems = std::move(other.mItems);
}

AccessorItem::AccessorItem(const DataBase &db, const rapidjson::Value &val, Block &parent)
  : ParentType(db, val, parent)
{
  int size = 1;
  JSONLOAD(sge::make_nvp("size", size));
  mItems.resize(size);
}

void AccessorItem::DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx, IContext & context)
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
  gui::DrawInventory::DrawInventorySlots(mItems, db, ItemContainerContext(), 2);
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
    if (auto empty = find_item(0))
    {
      std::get<0>(mItems[*empty]) = id;
      std::get<1>(mItems[*empty]) = count;
    }
    else
      return 0;
  }

  return count;
}

size_t AccessorItem::RemoveItem(ItemId id, size_t count)
{
  if (auto index = find_item(id))
  {
    if (count <= std::get<1>(mItems[*index])) 
      std::get<1>(mItems[*index]) -= count;

    if (std::get<1>(mItems[*index]) == 0)
    {
      std::get<0>(mItems[*index]) = 0;
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

const ItemList & AccessorItem::Items() const
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

