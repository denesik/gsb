#pragma once
#include <string>
#include "TemplateFactory.h"
#include <vector>
#include "IJsonSerializable.h"

class Chest;

class RecipeIn : public IJsonSerializable
{
public:
  ItemId id;
  int count = 1;

  // <"id"[, count]>
  void JsonLoad(BlocksDataBase& db, const rapidjson::Value& val) override;

  bool operator ==(const RecipeIn& other) const;
};

class RecipeOut : public IJsonSerializable
{
public:
  ItemId id;
  int count = 1;
  float chance = 1.0;

  // <"id"[, count[, chance]]>
  void JsonLoad(BlocksDataBase& db, const rapidjson::Value& val) override;

  bool operator ==(const RecipeOut& other) const;
};

class GSB_NOVTABLE IRecipe : public IJsonSerializable
{
public:
  virtual ~IRecipe() = default;

  virtual bool CanCraft(const Chest & c) const = 0;
  virtual bool Craft(const Chest & c) const = 0;
  virtual const std::vector<RecipeIn> & Components() const = 0;
  virtual const std::vector<RecipeOut> & Results() const = 0;
};

#define REGISTER_RECIPE_CLASS(type) REGISTER_ELEMENT(type, RecipeFactory::Get(), #type)

struct RecipeFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory<std::string, IRecipe>;
  static FactoryType& Get();
};