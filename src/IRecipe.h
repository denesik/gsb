#pragma once
#include <string>
#include "TemplateFactory.h"
#include <vector>
#include "IJsonSerializable.h"

class Chest;

class GSB_NOVTABLE IRecipe : public IJsonSerializable
{
public:
  virtual ~IRecipe() = default;

  virtual bool CanCraft(const Chest & c) const = 0;
  virtual bool Craft(const Chest & c) const = 0;
  virtual const std::vector<std::tuple<ItemId, size_t>> & Components() const = 0;
  virtual const std::vector<std::tuple<ItemId, size_t>> & Results() const = 0;
};

#define REGISTER_RECIPE(type) REGISTER_ELEMENT(type, RecipeFactory::Get(), #type)

struct RecipeFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory<std::string, IRecipe>;
  static FactoryType &Get();
};