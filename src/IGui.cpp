#include "IGui.h"

IGui::~IGui()
{
  //onGuiClose();
}

GuiCtx::GuiCtx(DataBase & db)
  : IDBHolder(db)
{
}

void GuiCtx::Reset()
{
  mStorage.clear();
}
