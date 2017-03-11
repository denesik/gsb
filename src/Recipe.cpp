#include "Recipe.h"
#include "Serialize.h"
#include "agent/agents/Chest.h"

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
  IRecipe::JsonLoad(db, val);
  JSONLOAD(sge::make_nvp("input", mComponents), sge::make_nvp("output", mResults));
}
