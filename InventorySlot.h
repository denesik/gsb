#pragma once
#include <tools\Common.h>
#include <tuple>
#include <vector>
#include <IGui.h>

class DataBase;

namespace gui
{
  void DrawInventorySlots(std::vector<std::tuple<ItemId, size_t>> &slot, const DataBase &db, GuiCtx & ctx, intptr_t caller);
}