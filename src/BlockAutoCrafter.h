#pragma once
#ifndef BlockAutoCrafter_h__
#define BlockAutoCrafter_h__


#include "BlockDynamicPart.h"

// Содержит 2 крафтера.
class BlockAutoCrafter : public BlockDynamicPart
{
public:
  BlockAutoCrafter();
  ~BlockAutoCrafter();
  BlockAutoCrafter(const BlockAutoCrafter &other);

  std::unique_ptr<BlockDynamicPart> Clone() override;

  void DrawGui(const Magnum::Timeline &dt) override;

  void Update() override;

private:


};

REGISTER_BLOCK_CLASS(BlockAutoCrafter);

#endif // BlockAutoCrafter_h__
