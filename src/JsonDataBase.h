#pragma once
#include <vector>
#include "BlockStaticPart.h"

class IDataBaseLoader
{
public:
  virtual ~IDataBaseLoader() = default;
  virtual void Load(const TextureAtlas &atlas, std::vector<std::unique_ptr<BlockStaticPart>> &storage) const = 0;
};

class JsonDataBase : public IDataBaseLoader
{
public:
  JsonDataBase(const std::string path);
  ~JsonDataBase();

  void Load(const TextureAtlas &atlas, std::vector<std::unique_ptr<BlockStaticPart>>& storage) const override;

private:
  std::string mPath;
};

