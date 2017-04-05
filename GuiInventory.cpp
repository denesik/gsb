#include "GuiInventory.h"
#include "imgui\imgui.h"
#include "DragNDrop.h"
#include <Creature.h>

void gui::DrawCaracterWindow(bool &opened, Creature & creature)
{
  ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiSetCond_FirstUseEver);
  ImGui::Begin("Caracter", &opened);
  ImGui::Text("Hello");
  ImGui::End();
}
