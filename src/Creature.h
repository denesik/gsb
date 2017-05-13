#pragma once
#ifndef Creature_h__
#define Creature_h__

#include "Movable.h"
#include <IGui.h>
#include <vector>
#include <ItemContainerContext.h>

class World;

class Creature : public Acceleratable, public ContextGui<PlayerInventoryContext>
{
public:
  using ContextType = PlayerInventoryContext;

  Creature(World &world);
  ~Creature();

  void Update(const Magnum::Timeline &tl);

  // Inherited via IGui
  virtual void DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx, IContext & context) override;
  ItemList & Inventory();
  ItemList & Hotbar();

private:
  World &mWorld;
  ItemList mInventory;
  ItemList mHotbar;
  int hotSelection;
};



#endif // Creature_h__
