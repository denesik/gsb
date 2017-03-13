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

class GSB_NOVTABLE IRecipe : public IJsonSerializable
{
public:
  using factory = TemplateFactory<std::string, IRecipe, void()>;

  virtual ~IRecipe() = default;

  virtual const std::vector<RecipeIn> & Components() const;
  virtual const std::vector<RecipeOut> & Results() const;

  void JsonLoad(DataBase & db, const rapidjson::Value& val) override;

  virtual RecipeId Id() const = 0;

  float Time() const;

  // TODO_Recipe 
  // � ������ ������������ ������� ������ ����� �������������.
  // ��������: � ���� ����������� �� json �������� ��� ������� ������� �� ����� ��������.
  // ��������� ����������� � �� - ��� ������. ��������� ���� �� �� � ��� - ������ ������ ��������������,
  // ��� �� ��������� ���� ���, �.�. �� �����������.
  // � ������ ����� ������� �� ����� �����������, ������ ����������� ���. 
  // ���� ��������� ������ ��������� �� ����� �����, ��� �� ������ ������������ ���� �� ����� ��������.
  //
  // � ���� �� ��� ���������� ������ ���� ������� ����� ��������� ����� � ��������� �����.
  // � ������ ��� �� �����.
  virtual std::unique_ptr<IRecipe> Clone() = 0; 

protected:
  float mTime = 0; // ����� � ��.
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

