#pragma once
#ifndef Creature_h__
#define Creature_h__

#include "Movable.h"
#include <IGui.h>

class World;

class Creature : public Movable, public IGui
{
public:
  Creature(World &world);
  ~Creature();

  void Update();

  // Inherited via IGui
  virtual void DrawGui(const Magnum::Timeline & dt) override;

private:
  World &mWorld;

};



#endif // Creature_h__
