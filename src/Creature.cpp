#include "Creature.h"
#include "World.h"



Creature::Creature(World &world)
  : mWorld(world)
{
}


Creature::~Creature()
{
}

void Creature::Update()
{
  auto old_pos = Movable::Pos();
  Movable::Update();
  auto new_pos = Movable::Pos();

  auto delta = new_pos - old_pos;

  if (delta.length() > 0)
  {
    auto dir = delta.normalized();
    auto sphere_checker = delta + dir * 0.2f;

    if (mWorld.GetBlockId(cs::WtoWB(old_pos + Magnum::Vector3(sphere_checker.x(), 0, 0))) == 0)
    {
      old_pos.x() += delta.x();
    }

    if (mWorld.GetBlockId(cs::WtoWB(old_pos + Magnum::Vector3(0, sphere_checker.y(), 0))) == 0)
    {
      old_pos.y() += delta.y();
    }

    if (mWorld.GetBlockId(cs::WtoWB(old_pos + Magnum::Vector3(0, 0, sphere_checker.z()))) == 0)
    {
      old_pos.z() += delta.z();
    }
  }

  if (false) // Выключили коллизии.
    Movable::SetPos(old_pos);
}

void Creature::DrawGui(const Magnum::Timeline & dt)
{
}
