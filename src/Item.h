#pragma once
#ifndef Item_h__
#define Item_h__

#include <stdint.h>
#include "IItem.h"

class Item : public IItem
{
public:
  Item();
  ~Item();


  void JsonLoad(BlocksDataBase & db, const rapidjson::Value& val) override;
};

REGISTER_ITEM_CLASS(Item);

#endif // Item_h__
