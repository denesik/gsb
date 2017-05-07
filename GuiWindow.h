#pragma once
#include <IGui.h>
#include <list>
#include <IDBHolder.h>

class GuiWindow : public IDBHolder
{
public:
  GuiWindow(DataBase &db, const std::string &name);

  void Draw(const Magnum::Timeline & dt);

  void AddGui(IGui & gui);

  void Reset();

  void Open();
  void Close();
  void Toggle();

private:

  std::list<GuiCtx::GuiLinkage> mLinkages;
  GuiCtx mCtx;

  bool mClosed = true;
  
  std::string mName;
};

