#include "GuiInventory.h"
#include "imgui\imgui.h"
#include "DragNDrop.h"
#include <Creature.h>
#include <DataBase.h>
#include "InventorySlot.h"

void gui::DrawCaracterWindow(bool &opened, Creature & creature, DataBase &db)
{
  ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiSetCond_FirstUseEver);
  ImGui::Begin("Caracter", &opened);
  ImGui::End();
}
