#include "GuiWindow.h"
#include <functional>
#include "imgui/imgui.h"
#include <Creature.h>

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
