#pragma once
#ifndef BlockAutoCrafter_h__
#define BlockAutoCrafter_h__


#include "BlockDynamicPart.h"


class BlockAutoCrafter : public BlockDynamicPart
{
public:
  BlockAutoCrafter();
  ~BlockAutoCrafter();
  BlockAutoCrafter(const BlockAutoCrafter &other);

  std::unique_ptr<BlockDynamicPart> Clone() override;


};

REGISTER_BLOCK_CLASS(BlockAutoCrafter);

#endif // BlockAutoCrafter_h__
