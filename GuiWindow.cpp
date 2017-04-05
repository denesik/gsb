#include "GuiWindow.h"
#include <functional>
#include "imgui/imgui.h"

GuiWindow::GuiWindow(const std::string & name) : mName(name)
{
}

void GuiWindow::Draw(const Magnum::Timeline & dt)
{
  if (mClosed)
    return;

  ImGui::Begin(mName.c_str());

  for(auto i = guis.begin(); i != guis.end(); ++i)
    (*i)->DrawGui(dt, mCtx);

  ImGui::End();
}

void GuiWindow::AddGui(IGui * gui)
{
  guis.push_back(gui);
  gui->onGuiClose.connect([this]() { mClosed = true; });
}

void GuiWindow::RemoveGui(IGui * gui)
{
  guis.remove(gui);
}

void GuiWindow::Reset()
{
  mCtx.Reset();
  guis.clear();
}

void GuiWindow::Open()
{
  mClosed = false;
}

void GuiWindow::Close()
{
  mClosed = true;
}
