#include "Recipe.h"
#include "Serialize.h"

bool Recipe::CanCraft(const Chest& c) const
{
  throw NotImplemented();
}

bool Recipe::Craft(const Chest& c) const
{
  throw NotImplemented();
}

const std::vector<RecipeIn> & Recipe::Components() const
{
  return mComponents;
}

const std::vector<RecipeOut> & Recipe::Results() const
{
  return mResults;
}

void Recipe::JsonLoad(BlocksDataBase & db, const rapidjson::Value& val)
{
  JSONLOAD(sge::make_nvp("input", mComponents), sge::make_nvp("output", mResults));
}
