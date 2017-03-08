#pragma once
#ifndef Item_h__
#define Item_h__

#include <stdint.h>
#include <rapidjson/document.h>
#include "TextureAtlas.h"
#include <Magnum/Magnum.h>
#include <Magnum/Math/Range.h>

class Item
{
public:
  Item();
  ~Item();

  void JsonLoad(const rapidjson::Value & val, const TextureAtlas &atlas);

  void GuiDraw();

private:
  Magnum::Range2D mTextureCoord;
  const size_t mTextId = 1;

};



#endif // Item_h__
