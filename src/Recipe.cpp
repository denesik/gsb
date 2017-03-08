#include "Recipe.h"

bool Recipe::CanCraft(const Chest& c) const
{
  throw NotImplemented();
}

bool Recipe::Craft(const Chest& c) const
{
  throw NotImplemented();
}

const std::vector<std::tuple<ItemId, size_t>> & Recipe::Components() const
{
  return mComponents;
}

const std::vector<std::tuple<ItemId, size_t>> & Recipe::Results() const
{
  return mComponents;
}

void Recipe::JsonLoad(const rapidjson::Value& val)
{
  throw NotImplemented();
}

RecipeFactory::FactoryType& RecipeFactory::Get()
{
  static FactoryType factory;
  return factory;
}
