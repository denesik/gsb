#pragma once
#include <IGui.h>
#include <list>
#include <IDBHolder.h>
#include <string>
#include <DataBase.h>

class Creature;

class GuiWindow : public DBHolder
{
public:
  GuiWindow(DataBase &db, const std::string &name);

  virtual void Draw(const Magnum::Timeline & dt);
  virtual ~GuiWindow() = default;

  GuiCtx::GuiLinkage & AddGui(IGui & gui);

  void Reset();

  void Open();
  void Close();
  void Toggle();
  const std::string &GetName();
  bool IsClosed();

private:

  std::list<std::unique_ptr<GuiCtx::GuiLinkage>> mLinkages;
  std::shared_ptr<GuiCtx> mCtx;

  bool mClosed = true;
  
  std::string mName;
};

class ItemListTransformGuiContext : public IContext
{
public:
  SortItemList::Params SortParams;
  bool Resort = false;
};

class ItemListTransformGui : public ContextGui<ItemListTransformGuiContext>
{
public:
  using ContextType = ItemListTransformGuiContext;

  // Унаследовано через ContextGui
  void DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx, IContext & context) override;

};

class ItemDBGuiContext : public IContext
{
public:
};

class ItemDBGui : public ContextGui<ItemDBGuiContext>
{
public:
  using ContextType = ItemDBGuiContext;

  // Унаследовано через ContextGui
  void DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx, IContext & context) override;

};

class GuiWindowPlayerInventory : public GuiWindow
{
public:
  GuiWindowPlayerInventory(Creature &player, DataBase &db, const std::string &name = "Player Inventory");

  ItemId BagSelection();
  ItemId HotbarSelection();
  void Sort();

  void Draw(const Magnum::Timeline & dt) override;

private:
  Creature &mPlayer;

  ItemListTransformGui mTransformGui;
  GuiCtx::GuiLinkage &mTransformLink;

  std::unique_ptr<SortItemList> mSort;

  GuiCtx::GuiLinkage &mPlayerGui;
};

class GuiWindowDb : public GuiWindow
{
  public:
    GuiWindowDb(DataBase &db, const std::string &name = "Data Base");

    void Draw(const Magnum::Timeline & dt) override;

private:
  ItemDBGui mItemDBGui;
  GuiCtx::GuiLinkage &mItemDBLink;
};