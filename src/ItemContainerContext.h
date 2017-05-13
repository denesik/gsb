#pragma once
#include <IGui.h>

class ItemContainerContext : public IContext
{
public:
  size_t Selected = 2;
};

class PlayerInventoryContext : public IContext
{
public:
  ItemContainerContext Bag = {};
  ItemContainerContext Hotbar = {};
};