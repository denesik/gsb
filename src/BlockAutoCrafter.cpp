#include "BlockAutoCrafter.h"



BlockAutoCrafter::BlockAutoCrafter()
{
}


BlockAutoCrafter::~BlockAutoCrafter()
{
}

BlockAutoCrafter::BlockAutoCrafter(const BlockAutoCrafter &other)
  : BlockDynamicPart(other)
{

}

std::unique_ptr<BlockDynamicPart> BlockAutoCrafter::Clone()
{
  BlockDynamicPart *ptr = new BlockAutoCrafter(*this);
  std::unique_ptr<BlockDynamicPart> obj;
  obj.reset(ptr);
  return std::move(obj);
}
