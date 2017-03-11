#pragma once
#ifndef BlockAutoCrafter_h__
#define BlockAutoCrafter_h__


#include "BlockDynamicPart.h"
#include "Timer.h"
#include "IRecipe.h"
#include "Crafter.h"

// Содержит 2 крафтера.
class BlockAutoCrafter : public BlockDynamicPart
{
public:
  BlockAutoCrafter();
  ~BlockAutoCrafter();
  BlockAutoCrafter(const BlockAutoCrafter &other);

  std::unique_ptr<BlockDynamicPart> Clone() override;

  void DrawGui(const Magnum::Timeline &dt) override;

  void Update(const Magnum::Timeline &dt) override;

private:
  Crafter mCrafter;
  Crafter mGenerator;
};

REGISTER_BLOCK_CLASS(BlockAutoCrafter);

#endif // BlockAutoCrafter_h__
