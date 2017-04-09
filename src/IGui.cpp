#include "IGui.h"

IGui::~IGui()
{
  //onGuiClose();
}

void GuiCtx::Reset()
{
  mStorage.clear();
}
