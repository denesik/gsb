#include "Item.h"



using namespace Magnum;

Item::Item()
{
}


Item::~Item()
{
}

void Item::JsonLoad(BlocksDataBase & db, const rapidjson::Value & val)
{
  if (val.HasMember("tex") && val["tex"].IsString())
  {
    const rapidjson::Value &arr = val["tex"];
    mTextureCoord = db.GetAtlasItems().GetTextureCoord(val["tex"].GetString()).value_or(Range2D{ Vector2{ 0.0f }, Vector2{ 1.0f } });
  }
}

const Magnum::Range2D & Item::TextureCoord() const
{
  return mTextureCoord;
}
