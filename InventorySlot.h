#pragma once
#include <tools\Common.h>
#include <tuple>
#include <vector>
#include <IGui.h>

class DataBase;
class ItemContainerContext;

namespace gui
{
  struct DrawInventory
  {
    static void DrawInventorySlots(ItemList &slot, const DataBase &db, ItemContainerContext & caller, int hor_size = 5);
  };
}