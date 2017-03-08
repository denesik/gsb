#include "Item.h"
#include "..\imgui\imgui.h"

using namespace Magnum;

Item::Item()
{
}


Item::~Item()
{
}

void Item::JsonLoad(BlocksDataBase & db, const rapidjson::Value & val)
{
  mTextureCoord = db.GetAtlasItems().GetTextureCoord("data/items/dirt.tga").value_or(Range2D{ Vector2{ 0.0f }, Vector2{ 1.0f } });
}

void Item::GuiDraw()
{
  ImGui::ImageButton(ImTextureID(mTextId), ImVec2(32, 32), ImVec2(mTextureCoord.left(), mTextureCoord.bottom()), ImVec2(mTextureCoord.right(), mTextureCoord.top()));
}
