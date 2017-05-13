#include "GuiWindow.h"
#include <functional>
#include "imgui/imgui.h"

GuiWindow::GuiWindow(DataBase &db, const std::string & name)
  : mName(name)
  , DBHolder(db)
{
  mCtx = std::shared_ptr<GuiCtx>(new GuiCtx(db));
}

void GuiWindow::Draw(const Magnum::Timeline & dt)
{
  if (mClosed)
    return;

  ImGui::Begin(mName.c_str());

  for (auto & link : mLinkages)
    link.DrawGui(dt);

  ImGui::End();
}

void GuiWindow::AddGui(IGui & gui)
{ 
  mLinkages.emplace_back(mCtx->Register(gui));
  //gui->onGuiClose.connect([this]() { Close(); Reset(); });
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
