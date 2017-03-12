#pragma once
#include <vector>
#include "StaticBlock.h"

class DataBase;

class IDataBaseLoader
{
public:
  virtual ~IDataBaseLoader() = default;
  virtual void Load(const TextureAtlas &atlas, DataBase &db) const = 0;
};

class JsonDataBase : public IDataBaseLoader
{
public:
  JsonDataBase(const std::string path);
  ~JsonDataBase();

  void Load(const TextureAtlas &atlas, DataBase &db) const override;

private:
  std::string mPath;
};

