#pragma once
#ifndef Creature_h__
#define Creature_h__

#include "Movable.h"
#include <IGui.h>
#include <vector>

class World;

class Creature : public Acceleratable, public NoContextGui
{
public:
  Creature(World &world);
  ~Creature();

  void Update(const Magnum::Timeline &tl);

  // Inherited via IGui
  virtual void DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx, IContext & context) override;
  std::vector<std::tuple<ItemId, size_t>> &Inventory();

private:
  World &mWorld;
  std::vector<std::tuple<ItemId, size_t>> mInventory;
  std::vector<std::tuple<ItemId, size_t>> mHotbar;
  int hotSelection;
};



#endif // Creature_h__
