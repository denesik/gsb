#pragma once
#ifndef BlocksDataBase_h__
#define BlocksDataBase_h__

#include <vector>
#include <memory>
#include "BlockStaticPart.h"
#include "TextureAtlas.h"


class BlocksDataBase
{
public:
  BlocksDataBase(const TextureAtlas &atlas);
  ~BlocksDataBase();

  const std::unique_ptr<BlockStaticPart> &GetBlockStaticPart(BlockId id) const;

private:
  std::vector<std::unique_ptr<BlockStaticPart>> mBlocks;

  const TextureAtlas &mAtlas;
};



#endif // BlocksDataBase_h__
