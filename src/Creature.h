#pragma once
#ifndef Creature_h__
#define Creature_h__

#include "Movable.h"
#include <IGui.h>
#include <vector>

class World;

class Creature : public Movable, public IGui
{
public:
  Creature(World &world);
  ~Creature();

  void Update();

  // Inherited via IGui
  virtual void DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx) override;

private:
  World &mWorld;
  std::vector<std::tuple<ItemId, size_t>> mInventory;
};



#endif // Creature_h__
