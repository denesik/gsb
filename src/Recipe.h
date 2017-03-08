#pragma once
#include "TemplateFactory.h"
#include "agent/Agent.h"
#include <vector>
#include "IRecipe.h"

class Chest;

class Recipe : public IRecipe
{
public:
  bool CanCraft(const Chest & c) const override;
  bool Craft(const Chest & c) const override;
  const std::vector<RecipeIn> & Components() const override;
  const std::vector<RecipeOut> & Results() const override;

  void JsonLoad(BlocksDataBase & db, const rapidjson::Value& val) override;

private:
  std::vector<RecipeIn> mComponents;
  std::vector<RecipeOut> mResults;
};

REGISTER_RECIPE_CLASS(Recipe);