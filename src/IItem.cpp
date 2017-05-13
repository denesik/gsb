#include "IItem.h"

const std::string & IItem::GetName() const
{
	return mName;
}

const std::string & IItem::GetDescription() const
{
	return mDescription;
}

void IItem::SetName(const std::string & name)
{
	mName = name;
}

void IItem::SetDescription(const std::string & descriprion)
{
	mDescription = descriprion;
}

void IItem::SetBlock(BlockId b)
{
  mBlock = b;
}

BlockId IItem::GetBlock() const
{
  return mBlock;
}
