#pragma once
#include <IGui.h>
#include <list>
#include <IDBHolder.h>

class GuiWindow : public IDBHolder
{
public:
  GuiWindow(DataBase &db, const std::string &name);

  void Draw(const Magnum::Timeline & dt);

  void AddGui(IGui * gui);
  void RemoveGui(IGui * gui);
  void Reset();
  void Open();
  void Close();
  void Toggle();

private:
  std::list<IGui *> guis;
  bool mClosed = true;
  GuiCtx mCtx;
  std::string mName;
};

