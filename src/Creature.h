#pragma once
#ifndef Creature_h__
#define Creature_h__

#include "Movable.h"

class World;

class Creature : public Movable
{
public:
  Creature(World &world);
  ~Creature();

  void Update();

private:
  World &mWorld;

};



#endif // Creature_h__
