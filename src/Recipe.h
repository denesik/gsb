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
  const std::vector<std::tuple<ItemId, size_t>> & Components() const override;
  const std::vector<std::tuple<ItemId, size_t>> & Results() const override;

  void JsonLoad(const rapidjson::Value& val) override;

private:
  std::vector<std::tuple<ItemId, size_t>> mComponents;
  std::vector<std::tuple<ItemId, size_t>> mResults;
};

REGISTER_RECIPE(Recipe);