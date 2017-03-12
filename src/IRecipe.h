#pragma once
#include <string>
#include "TemplateFactory.h"
#include <vector>
#include "IJsonSerializable.h"

class AccessorItem;

using RecipeId = unsigned int;

class RecipeIn : public IJsonSerializable
{
public:
  ItemId id;
  int count = 1;

  // <"id"[, count]>
  void JsonLoad(DataBase& db, const rapidjson::Value& val) override;

  bool operator ==(const RecipeIn& other) const;
};

class RecipeOut : public IJsonSerializable
{
public:
  ItemId id;
  int count = 1;
  float chance = 1.0;

  // <"id"[, count[, chance]]>
  void JsonLoad(DataBase& db, const rapidjson::Value& val) override;

  bool operator ==(const RecipeOut& other) const;
};

class GSB_NOVTABLE IRecipe : public IJsonSerializable
{
public:
  virtual ~IRecipe() = default;

  virtual const std::vector<RecipeIn> & Components() const;
  virtual const std::vector<RecipeOut> & Results() const;

  void JsonLoad(DataBase & db, const rapidjson::Value& val) override;

  virtual RecipeId Id() const = 0;

  float Time() const;

  // TODO_Recipe 
  // В случае наследования рецепты должны уметь клонироваться.
  // Ситуация: в блок автокрафтер из json задается тип рецепта который он умеет крафтить.
  // Загружаем автокрафтер в бд - все хорошо. Клонируем блок из бд в мир - рецепт должен склонироваться,
  // что бы сохранить свой тип, т.к. он полиморфный.
  // В случае тегов рецепты не нужно клонировать, просто скопируется тег. 
  // Даже фиктивный рецепт наверняка не нужен будет, что бы задать автокрафтеру кого он умеет крафтить.
  //
  // К тому же для добавления нового типа рецепта нужно создавать класс и компилить прогу.
  // С тегами это не нужно.
  virtual std::unique_ptr<IRecipe> Clone() = 0; 

protected:
  float mTime = 0; // Время в мс.
  std::vector<RecipeIn> mComponents;
  std::vector<RecipeOut> mResults;
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