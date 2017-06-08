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

GuiWindowAutocraft::GuiWindowAutocraft(Creature &player, DataBase & db, const std::string & name)
  : GuiWindow(db, name)
  , mPlayer(player)
{
}

void DrawSome(const DataBase &db, const ItemId &s, const Magnum::Timeline & dt, ImColor c = { 0,0,0,0 })
{
  //static float tt = 0;
  //tt += gt;

  //if (s.find("tag_") != -1)
  //{
  //  auto list = DB::Get().Taglist(s);
  //  srand(int(tt));
  //  if (!list.empty())
  //    ss = list[0];
  //}


  const auto &db_item = db.GetItem(s);
  if (db_item)
  {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const auto &coord = static_cast<const Item &>(*db_item).TextureCoord();

    ImGui::ImageButton(
      ImTextureID(1),
      { 32, 32 },
      { coord.left(), coord.top() },
      { coord.right(), coord.bottom() },
      -1,
      { 0, 0, 0, 0 }
    );

    if (ImGui::IsItemHovered())
    {
      //ImGui::SetTooltip("%s\n%s", a->first.get().c_str(), std::get<0>(a.second)->GetDescription().c_str());
    }
  }
}


void DrawGui(const IRecipe &recipe, const Magnum::Timeline & dt, GuiCtx & ctx)
{
  bool first = true;
  bool pressed = false;
  for (const auto &inp : recipe.Components())
  {
    if (first)
      first = false;
    else
      ImGui::SameLine();

    DrawSome(ctx.GetDataBase(), inp.id, dt);
    auto draw_list = ImGui::GetWindowDrawList();
    auto rectMax = ImGui::GetItemRectMax();
    if (inp.count >= 100)
      draw_list->AddText(ImVec2(rectMax.x - 22, rectMax.y - 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(inp.count).c_str());
    else if (inp.count >= 10)
      draw_list->AddText(ImVec2(rectMax.x - 16, rectMax.y - 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(inp.count).c_str());
    else
      draw_list->AddText(ImVec2(rectMax.x - 10, rectMax.y - 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(inp.count).c_str());
  }

  //{
  //  auto atl = TextureManager::Get().GetTexture("arrow_right");
  //  auto &tex = std::get<0>(atl);
  //  auto &atluv = std::get<1>(atl);

  //  auto uv = glm::vec2(atluv.x, atluv.y);
  //  auto uv2 = glm::vec2(atluv.z, atluv.w);

  //  ImGui::SameLine();
  //  ImGui::ImageButton(reinterpret_cast<ImTextureID>(tex->GetId()), { 32,32 }, uv2, uv);
  //  if (ImGui::IsItemHovered())
  //  {
  //    ImGui::SetTooltip("craft this recipe");
  //    if (ImGui::IsMouseClicked(0))
  //      pressed = true;
  //  }
  //}

  for (const auto &out : recipe.Results())
  {
    ImGui::SameLine();
    DrawSome(ctx.GetDataBase(), out.id, dt);
    auto draw_list = ImGui::GetWindowDrawList();
    auto rectMax = ImGui::GetItemRectMax();
    if (out.count >= 100)
      draw_list->AddText(ImVec2(rectMax.x - 22, rectMax.y - 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(out.count).c_str());
    else if (out.count >= 10)
      draw_list->AddText(ImVec2(rectMax.x - 16, rectMax.y - 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(out.count).c_str());
    else
      draw_list->AddText(ImVec2(rectMax.x - 10, rectMax.y - 13), ImGui::GetColorU32(ImGuiCol_Text), std::to_string(out.count).c_str());
  }

}

void GuiWindowAutocraft::Draw(const Magnum::Timeline & dt)
{
  if (IsClosed())
    return;

  const auto deque = mPlayer.GetRecipeDeque();
  const auto & top = (*deque.begin()).get();
  DrawGui(top, dt, *mCtx);

  GuiWindow::Draw(dt);
}
