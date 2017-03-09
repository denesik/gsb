#include "Recipe.h"
#include "Serialize.h"
#include "agent/agents/Chest.h"

bool Recipe::CanCraft(const Chest& c) const
{
  throw NotImplemented();
}

bool Recipe::Craft(Chest& chest) const
{
  // Ищем нужные компоненты.
  // Если присутствуют все - удаляем их.
  // Добавляем новую компоненту.

  const auto &items = chest.Items();
  for (const auto &c : mComponents)
  {
    bool found = false;
    for (const auto &i : items)
    {
      if (c.id == std::get<0>(i))
      {
        found = true;
        break;
      }
    }
    if (!found)
    {
      return false;
    }
  }

  for (const auto &c : mComponents)
    chest.RemoveItem(c.id, 0);

  for (const auto &r : mResults)
    chest.AddItem(r.id, 0);

  return true;
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
