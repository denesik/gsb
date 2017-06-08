#include "Creature.h"
#include "World.h"
#include "../InventorySlot.h"
#include "../imgui/imgui.h"
#include <IRecipe.h>

Creature::Creature(World &world)
  : mWorld(world)
  , mInventory({ 
    { 1,1 },
    { 2,1 },
    { 3,1 },
    { },
    {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}
  })
  , mHotbar({ 
    { 1, 1 },
    { },
    { },
    { },
    { 350, 1 }
  })
{
}


Creature::~Creature()
{
}

void Creature::Update(const Magnum::Timeline &tl)
{
  auto old_pos = Movable::Pos();
  Acceleratable::Update(tl);
  auto new_pos = Movable::Pos();

  auto delta = new_pos - old_pos;

  if (delta.length() > 0)
  {
    auto dir = delta.normalized();
    auto sphere_checker = delta + dir * 0.2f;

    if (mWorld.GetBlockId(cs::WtoWB(old_pos + Magnum::Vector3(sphere_checker.x(), 0, 0))) == 1)
    {
      old_pos.x() += delta.x();
    }
    else
    {
      Velocity().x() = 0;
    }

    if (mWorld.GetBlockId(cs::WtoWB(old_pos + Magnum::Vector3(0, sphere_checker.y(), 0))) == 1)
    {
      old_pos.y() += delta.y();
    }
    else
    {
      Velocity().y() = 0;
    }

    if (mWorld.GetBlockId(cs::WtoWB(old_pos + Magnum::Vector3(0, 0, sphere_checker.z()))) == 1)
    {
      old_pos.z() += delta.z();
    }
    else
    {
      Velocity().z() = 0;
    }
  }

  //if (false) // Выключили коллизии.
    Movable::SetPos(old_pos);
}

void Creature::DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx, IContext & context)
{
  auto &inventoryContext = static_cast<PlayerInventoryContext &>(context);
  const auto &db = ctx.GetDataBase();

  gui::DrawInventory::DrawInventorySlots(mInventory, db, inventoryContext.Bag, 5);
  ImGui::Separator();
  gui::DrawInventory::DrawInventorySlots(mHotbar, db, inventoryContext.Hotbar, 5);
}

ItemList & Creature::Inventory()
{
  return mInventory;
}

ItemList & Creature::Hotbar()
{
  return mHotbar;
}

const std::deque<std::reference_wrapper<IRecipe>>& Creature::GetRecipeDeque() const
{
  return mRecipeDeque;
}
