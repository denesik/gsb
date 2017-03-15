#pragma once
#include <string>
#include "TemplateFactory.h"
#include <vector>
#include "IJsonSerializable.h"


class RecipeIn : public IJsonSerializable
{
public:
  ItemId id;
  size_t count = 1;

  // <"id"[, count]>
  void JsonLoad(DataBase& db, const rapidjson::Value& val) override;

  bool operator ==(const RecipeIn& other) const;
};

class RecipeOut : public IJsonSerializable
{
public:
  ItemId id;
  size_t count = 1;
  float chance = 1.0;

  // <"id"[, count[, chance]]>
  void JsonLoad(DataBase& db, const rapidjson::Value& val) override;

  bool operator ==(const RecipeOut& other) const;
};



class IRecipe : public IJsonSerializable
{
public:
  using Tag = unsigned int;
  using factory = TemplateFactory<std::string, IRecipe, void()>;

  virtual ~IRecipe() = default;

  void JsonLoad(DataBase & db, const rapidjson::Value& val) override;

  virtual const std::vector<RecipeIn> & Components() const;
  virtual const std::vector<RecipeOut> & Results() const;

  float Time() const;

protected:
  float mTime = 0; // ¬рем€ в мс.
  std::vector<RecipeIn> mComponents;
  std::vector<RecipeOut> mResults;
};


