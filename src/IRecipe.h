#pragma once
#include <string>
#include "TemplateFactory.h"
#include <vector>
#include "IJsonSerializable.h"

class Chest;

using RecipeId = unsigned int;

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

  virtual const std::vector<RecipeIn> & Components() const = 0;
  virtual const std::vector<RecipeOut> & Results() const = 0;

  void JsonLoad(BlocksDataBase & db, const rapidjson::Value& val) override;

  virtual RecipeId Id() const = 0;

  float Time() const;

protected:
  float mTime = 0; // ¬рем€ в мс.
};

template <RecipeId aId>
class INumeredRecipe : public IRecipe
{
public:
  static constexpr RecipeId TypeId() { return aId; }

  RecipeId Id() const override { return aId; }
};

#define REGISTER_RECIPE_CLASS(type) REGISTER_ELEMENT(type, RecipeFactory::Get(), #type)

struct RecipeFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory<std::string, IRecipe>;
  static FactoryType& Get();
};