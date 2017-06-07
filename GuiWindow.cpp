#include "GuiWindow.h"
#include <functional>
#include "imgui/imgui.h"
#include <Creature.h>
#include <Item.h>

GuiWindow::GuiWindow(DataBase &db, const std::string & name)
  : mName(name)
  , DBHolder(db)
  , mCtx(std::make_shared<GuiCtx>(db))
{
}

void GuiWindow::Draw(const Magnum::Timeline & dt)
{
  if (mClosed)
    return;

  ImGui::Begin(mName.c_str());

  for (auto & link : mLinkages)
    link->DrawGui(dt);

  ImGui::End();
}

GuiCtx::GuiLinkage & GuiWindow::AddGui(IGui & gui)
{
  mLinkages.push_back(std::move(mCtx->Register(gui)));
  return **(--mLinkages.end());
}

void GuiWindow::Reset()
{
  mLinkages.clear();
}

void GuiWindow::Open()
{
  mClosed = false;
}

void GuiWindow::Close()
{
  mClosed = true;
}

void GuiWindow::Toggle()
{
  if (mClosed)
    Open();
  else
    Close();
}

const std::string & GuiWindow::GetName()
{
  return mName;
}

bool GuiWindow::IsClosed()
{
  return mClosed;
}

GuiWindowPlayerInventory::GuiWindowPlayerInventory(Creature &player, DataBase & db, const std::string & name)
  : GuiWindow(db, name)
  , mPlayer(player)
  , mTransformLink(AddGui(mTransformGui))
  , mSort(std::make_unique<SortItemList>(mTransformLink.Context<ItemListTransformGui::ContextType>().SortParams))
  , mPlayerGui(AddGui(player))
{
}

ItemId GuiWindowPlayerInventory::BagSelection()
{
  const auto sel = mPlayerGui.Context<Creature::ContextType>().Bag.Selected;
  return std::get<0>(mPlayer.Inventory()[sel]);
}

ItemId GuiWindowPlayerInventory::HotbarSelection()
{
  const auto sel = mPlayerGui.Context<Creature::ContextType>().Hotbar.Selected;
  return std::get<0>(mPlayer.Hotbar()[sel]);
}

void GuiWindowPlayerInventory::Sort()
{
  if (mSort)
  {
    (*mSort)(mPlayer.Inventory());
  }
}

void GuiWindowPlayerInventory::Draw(const Magnum::Timeline & dt)
{
  if (IsClosed())
    return;

  auto &resort = mTransformLink.Context<ItemListTransformGui::ContextType>().Resort;
  if (resort)
    Sort();
  resort = false;

  GuiWindow::Draw(dt);
}

void ItemListTransformGui::DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx, IContext & c)
{
  auto &context = static_cast<ContextType &>(c);

  if (ImGui::Button("Sort"))
  {
    context.Resort = true;
  }
  ImGui::SameLine();
  if (ImGui::Button("Order"))
  {
    if (context.SortParams.IsIncreasing())
      context.SortParams.SetDecreaseSort();
    else
      context.SortParams.SetIncreaseSort();
  }
}

void ItemDBGui::DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx, IContext & context)
{
  const auto &dbItems = ctx.GetDataBase().GetItems();
  const auto &dbItemsInterval = ctx.GetDataBase().GetLoadedItemsInterval();
  const size_t count = ctx.GetDataBase().GetItemCount();
  constexpr int hor_size = 6;

  int curItem = 0;
  for (const auto &itemInterval : dbItemsInterval)
  {
    for (size_t index = itemInterval.first.lower(); index < itemInterval.first.upper(); ++index)
    {
      ImVec2 base_pos = ImGui::GetCursorScreenPos();
      ImDrawList* draw_list = ImGui::GetWindowDrawList();

      const auto &db_item = dbItems[index];

      Magnum::Range2D coord = static_cast<const Item &>(*db_item).TextureCoord();

      ImGui::ImageButton(
        ImTextureID(1),
        { 32, 32 },
        { coord.left(), coord.top() },
        { coord.right(), coord.bottom() },
        -1,
        { 0, 0, 0, 0 }
      );

      if (ImGui::IsItemHovered())
        ImGui::SetTooltip("%s \n%s", db_item->GetName().c_str(), db_item->GetDescription().c_str());

      if (((curItem + 1 + hor_size) % hor_size) != 0 && curItem != count - 1) ImGui::SameLine();
      curItem++;
    }
  }
}

GuiWindowDb::GuiWindowDb(DataBase & db, const std::string & name)
  : GuiWindow(db, name)
  , mItemDBLink(AddGui(mItemDBGui))
{
}

void GuiWindowDb::Draw(const Magnum::Timeline & dt)
{
  if (IsClosed())
    return;

  GuiWindow::Draw(dt);
}
