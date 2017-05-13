#include "InventorySlot.h"
#include "DragNDrop.h"
#include <Item.h>
#include "imgui\imgui.h"
#include <Creature.h>

constexpr float selectionTickness = 2;
constexpr float halfSelectionTickness = selectionTickness / 2.f;

boost::optional<size_t> find_item(ItemList &slots, ItemId id)
{
  auto it = std::find_if(slots.begin(), slots.end(), [id](const auto &val)
  {
    return std::get<0>(val) == id;
  });

  if (it != slots.end())
    return (std::distance(slots.begin(), it));

  return{};
}

void gui::DrawInventory::DrawInventorySlots(ItemList &slots, const DataBase &db, ItemContainerContext & context, int hor_size)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  for (size_t curItem = 0; curItem < slots.size(); curItem++)
  {
    ImVec2 base_pos = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    Magnum::Range2D &coord = Magnum::Range2D(Magnum::Vector2(0,0), Magnum::Vector2(0, 0));

    bool has_item = std::get<0>(slots[curItem]) != 0;
    const auto &db_item = db.GetItem(std::get<0>(slots[curItem]));

    if (has_item)
    {
      coord = static_cast<const Item &>(*db_item).TextureCoord();
    }

    ImGui::ImageButton(
      ImTextureID(1),
      { 32, 32 },
      { coord.left(), coord.top() },
      { coord.right(), coord.bottom() },
      -1,
      { 0, 0, 0, 0 }
    );

    if (ImGui::IsItemHovered() && ((ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1))))
    {
      context.Selected = curItem;
    }

    if (ImGui::IsItemHovered())
    {
      if (DragNDrop::IsBusy() && !ImGui::IsMouseDown(0))
      {
        std::vector<std::tuple<ItemId, size_t>> dropped = DragNDrop::Drop();

        if (dropped.size() == 1 && std::get<0>(slots[curItem]) == 0)
        {
          //если слот один -- помещаем его в позицию под курсором
          std::get<0>(slots[curItem]) = std::get<0>(dropped[0]);
          std::get<1>(slots[curItem]) = std::get<1>(dropped[0]);
        }
        else for (size_t j = 0; j < dropped.size(); j++)
        {
          if (auto index = find_item(slots, std::get<0>(dropped[j])))
          {
            //нашли совпадающий -- добавляем
            std::get<1>(slots[*index]) += std::get<1>(dropped[j]);
          }
          else
          {
              if (auto empty = find_item(slots, 0))
              {
                //нашли пустой -- засовываем
                std::get<0>(slots[*empty]) = std::get<0>(dropped[j]);
                std::get<1>(slots[*empty]) = std::get<1>(dropped[j]);
              }
              else
              {
              } //TODO: вываливать лишние
          }
        }
      }
      else if (ImGui::IsMouseDragging(0) && !DragNDrop::IsBusy())
      {
        std::vector<std::tuple<ItemId, size_t>> dragged;
        std::vector<std::tuple<ItemId, size_t>> new_items;

        for (size_t j = 0; j < slots.size(); j++)
        {
          if (curItem == j)
          {
            dragged.push_back(slots[j]);
            new_items.push_back({});
          }
          else
            new_items.push_back(slots[j]);
        }
        slots = new_items;
        DragNDrop::Drag(std::move(dragged));
      }
    }

    if (has_item)
    {
      ImVec2 text_offset;
      if (std::get<1>(slots[curItem]) >= 100)
        text_offset = ImVec2(ImGui::GetItemRectMax().x - 22, ImGui::GetItemRectMax().y - 13);
      else if (std::get<1>(slots[curItem]) >= 10)
        text_offset = ImVec2(ImGui::GetItemRectMax().x - 16, ImGui::GetItemRectMax().y - 13);
      else
        text_offset = ImVec2(ImGui::GetItemRectMax().x - 10, ImGui::GetItemRectMax().y - 13);
      draw_list->AddText(text_offset, ImGui::GetColorU32(ImGuiCol_Text), std::to_string(std::get<1>(slots[curItem])).c_str());
    }

    if (ImGui::IsItemHovered() && has_item)
      ImGui::SetTooltip("%s x%d\n%s", db_item->GetName().c_str(), std::get<1>(slots[curItem]), db_item->GetDescription().c_str());


    if (context.Selected == curItem)
    {
      draw_list->AddRect(
        ImVec2(base_pos.x - halfSelectionTickness, base_pos.y - halfSelectionTickness),
        ImVec2(base_pos.x + 32 + 4 * 2 + halfSelectionTickness, base_pos.y + 32 + 3 * 2 + halfSelectionTickness),
        IM_COL32(255, 255, 255, 255),
        0,
        -1,
        selectionTickness
      );
    }
    else
    {
      draw_list->AddRect(base_pos, ImVec2(base_pos.x + 32 + 4 * 2, base_pos.y + 32 + 3 * 2), IM_COL32(255, 255, 255, 100));
    }

    if (((curItem + 1 + hor_size) % hor_size) != 0) ImGui::SameLine();
  }
}
