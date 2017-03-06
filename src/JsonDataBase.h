#pragma once
#include <vector>
#include "BlockStaticPart.h"

class BlocksDataBase;

class IDataBaseLoader
{
public:
  virtual ~IDataBaseLoader() = default;
  virtual void Load(const TextureAtlas &atlas, BlocksDataBase &db) const = 0;
};

class JsonDataBase : public IDataBaseLoader
{
public:
  JsonDataBase(const std::string path);
  ~JsonDataBase();

  void Load(const TextureAtlas &atlas, BlocksDataBase &db) const override;

private:
  std::string mPath;
};

