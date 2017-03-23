#include "Accessor.h"
#include <Magnum/Timeline.h>
#include "../../imgui/imgui.h"
#include "../Block.h"
#include <rapidjson/document.h>
#include "../tools/Crc32.h"

Accessor::Accessor(const Accessor &other)
  : Accessor(other, other.mParent)
{
}

Accessor::Accessor(Accessor &&other)
  : Accessor(std::move(other), other.mParent)
{
}

Accessor::Accessor(const Accessor &other, Block &parent)
  : mParent(parent), mSides(other.mSides), m_name(other.m_name)
{

}

Accessor::Accessor(Accessor &&other, Block &parent)
  : mParent(parent), mSides(std::move(other.mSides)), m_name(other.m_name)
{

}

Accessor::Accessor(const DataBase &db, const rapidjson::Value &val, Block &parent)
  : mParent(parent), m_name(LoadName(val))
{
  if (val.HasMember("bindings") && val["bindings"].IsArray())
  {
    const auto& bindings = val["bindings"];
    for (decltype(bindings.Size()) b = 0; b < bindings.Size(); b++)
    {
      std::string directions = bindings[b].GetString();
      SideIndex side;
      switch (directions[0])
      {
      case 'r': side = IWEST; break;
      case 'l': side = IEAST; break;
      case 't': side = ITOP; break;
      case 'd': side = IDOWN; break;
      case 'f': side = ISOUTH; break;
      case 'b': side = INORTH; break;
      default: continue; //TODO: материться
      }
      if (directions[1] != '_') mSides[side] = static_cast<AccessorDirection>(mSides[side] | AccessorDirection::in);
      if (directions[2] != '_') mSides[side] = static_cast<AccessorDirection>(mSides[side] | AccessorDirection::out);
    }
  }
  else
  {
    mSides[INORTH] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
    mSides[ISOUTH] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
    mSides[IEAST] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
    mSides[IWEST] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
    mSides[ITOP] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
    mSides[IDOWN] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
  }
}

Block &Accessor::Parent() const
{
  return mParent;
}

AccessorDirection Accessor::GetDirection(SideIndex side)
{
  return mSides[side];
}

AccessorId Accessor::Name() const
{
  return m_name;
}

AccessorId Accessor::NameFromString(const std::string &name)
{
  return gsb::crc32<std::string>()(name);
}

AccessorId Accessor::LoadName(const rapidjson::Value &val)
{
  if (val.HasMember("name") && val["name"].IsString())
  {
    return NameFromString(val["name"].GetString());
  }
  return 0;
}

