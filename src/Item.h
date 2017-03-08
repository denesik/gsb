#pragma once
#ifndef Item_h__
#define Item_h__

#include <stdint.h>
#include <rapidjson/document.h>
#include "TextureAtlas.h"
#include <Magnum/Magnum.h>
#include <Magnum/Math/Range.h>
#include "BlocksDataBase.h"
#include "IGui.h"

class Item : public IItem/*, public IGui*/
{
public:
  Item();
  ~Item();

  void JsonLoad(BlocksDataBase & db, const rapidjson::Value & val) override;

  void DrawGui(const Magnum::Timeline &dt) const /*override*/;

private:
  Magnum::Range2D mTextureCoord;
  const size_t mTextId = 1;

};

REGISTER_ITEM_CLASS(Item);

#endif // Item_h__
