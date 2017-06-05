#pragma once
#include "GuiWindow.h"

class GuiWindowOptions : public GuiWindow
{
public:
  GuiWindowOptions(Creature &player, DataBase &db, const std::string &name = "Player Inventory");

  ItemId BagSelection();
  ItemId HotbarSelection();
  void Sort();

  void Draw(const Magnum::Timeline & dt) override;

private:
  Creature &mPlayer;

  ItemListTransformGui mTransformGui;
  GuiCtx::GuiLinkage &mTransformLink;

  ItemDBGui mItemDBGui;
  GuiCtx::GuiLinkage &mItemDBLink;

  std::unique_ptr<SortItemList> mSort;

  GuiCtx::GuiLinkage &mPlayerGui;
};