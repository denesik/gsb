#pragma once
#include "TemplateFactory.h"
#include "agent/Agent.h"
#include <vector>
#include "Item.h"

class Chest;

class IRecipe : public IJsonSerializable
{
public:
  virtual ~IRecipe() = default;

  virtual bool CanCraft(const Chest & c) const = 0;
  virtual bool Craft(const Chest & c) const = 0;
  virtual const std::vector<std::tuple<ItemId, size_t>> & Components() const = 0;
  virtual const std::vector<std::tuple<ItemId, size_t>> & Results() const = 0;
};

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

#define REGISTER_AGENT(type) REGISTER_ELEMENT(type, AgentFactory::Get(), #type)

struct RecipeFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory<std::string, Recipe>;
  static FactoryType &Get();
};