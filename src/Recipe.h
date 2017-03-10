#pragma once
#include "TemplateFactory.h"
#include "agent/Agent.h"
#include <vector>
#include "IRecipe.h"
#include "tools/Crc32.h"

class Chest;

class Recipe : public INumeredRecipe<gsb::crc32<std::string>()("Recipe")>
{
public:
  bool CanCraft(const Chest & c) const override;
  bool Craft(Chest & c) const override;
  const std::vector<RecipeIn> & Components() const override;
  const std::vector<RecipeOut> & Results() const override;

  void JsonLoad(BlocksDataBase & db, const rapidjson::Value& val) override;

private:
  std::vector<RecipeIn> mComponents;
  std::vector<RecipeOut> mResults;
};

REGISTER_RECIPE_CLASS(Recipe);