#pragma once
#ifndef Item_h__
#define Item_h__

#include <stdint.h>
#include <rapidjson/document.h>
#include "TextureAtlas.h"
#include <Magnum/Magnum.h>
#include <Magnum/Math/Range.h>
#include "BlocksDataBase.h"

class Item : public IItem
{
public:
  Item();
  ~Item();

  void JsonLoad(BlocksDataBase & db, const rapidjson::Value & val) override;

  const Magnum::Range2D &TextureCoord() const;

private:
  Magnum::Range2D mTextureCoord;
  const size_t mTextId = 1;

};

REGISTER_ITEM_CLASS(Item);

#endif // Item_h__
