#include "IRecipe.h"
#include <DataBase.h>
#include "Serialize.h"

void RecipeIn::JsonLoad(DataBase& db, const rapidjson::Value& val)
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

void RecipeOut::JsonLoad(DataBase& db, const rapidjson::Value& val)
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

void IRecipe::JsonLoad(DataBase & db, const rapidjson::Value& val)
{
  JSONLOAD(sge::make_nvp("time", mTime));
  JSONLOAD(sge::make_nvp("input", mComponents), sge::make_nvp("output", mResults));
}

float IRecipe::Time() const
{
  return mTime;
}

const std::vector<RecipeIn> & IRecipe::Components() const
{
  return mComponents;
}

const std::vector<RecipeOut> & IRecipe::Results() const
{
  return mResults;
}