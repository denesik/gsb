#include "IRecipe.h"
#include <BlocksDataBase.h>

void RecipeIn::JsonLoad(BlocksDataBase& db, const rapidjson::Value& val)
{
  std::string name = val.Begin()->GetString();
  id = db.ItemIdFromName(name);

  if (val.Capacity() >= 2)
    count = val[1].GetInt();
}

bool RecipeIn::operator==(const RecipeIn& other) const
{
  if (&other == this)
    return true;
  return id == other.id && count == other.count;
}

void RecipeOut::JsonLoad(BlocksDataBase& db, const rapidjson::Value& val)
{
  std::string name = val.Begin()->GetString();
  id = db.ItemIdFromName(name);

  if (val.Capacity() >= 2)
    count = val[1].GetInt();
  if (val.Capacity() >= 3)
    chance = val[2].GetDouble();
}

bool RecipeOut::operator==(const RecipeOut& other) const
{
  if (&other == this)
    return true;
  return id == other.id && count == other.count && chance == other.chance;
}

RecipeFactory::FactoryType& RecipeFactory::Get()
{
  static FactoryType factory;
  return factory;
}