#pragma once
#include <IGui.h>
#include <list>

class GuiWindow
{
public:
  GuiWindow(const std::string &name);

  void Draw(const Magnum::Timeline & dt);

  void AddGui(IGui * gui);
  void RemoveGui(IGui * gui);
  void Reset();
  void Open();
  void Close();

private:
  std::list<IGui *> guis;
  bool mClosed = false;
  GuiCtx mCtx;
  std::string mName;
};

