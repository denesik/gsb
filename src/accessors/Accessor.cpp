#include "Accessor.h"
#include <Magnum/Timeline.h>
#include "../../imgui/imgui.h"
#include "../Block.h"
#include <rapidjson/document.h>

Accessor::Accessor(const Accessor &other)
  : mParent(other.mParent), mSides(other.mSides)
{
}

Accessor::Accessor(Accessor &&other)
  : mParent(other.mParent), mSides(std::move(other.mSides))
{
}

Accessor::Accessor(const Accessor &other, Block &parent)
  : mParent(parent), mSides(other.mSides)
{

}

Accessor::Accessor(Accessor &&other, Block &parent)
  : mParent(parent), mSides(std::move(other.mSides))
{

}

Accessor::Accessor(const DataBase &db, const rapidjson::Value &val, Block &parent)
  : mParent(parent)
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
      case 'r': side = IRIGHT; break;
      case 'l': side = ILEFT; break;
      case 't': side = ITOP; break;
      case 'd': side = IBOTTOM; break;
      case 'f': side = IFRONT; break;
      case 'b': side = IBACK; break;
      default: continue; //TODO: материться
      }
      if (directions[1] != '_') mSides[side] = static_cast<AccessorDirection>(mSides[side] | AccessorDirection::in);
      if (directions[2] != '_') mSides[side] = static_cast<AccessorDirection>(mSides[side] | AccessorDirection::out);
    }
  }
  else
  {
    mSides[IBACK] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
    mSides[IFRONT] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
    mSides[ILEFT] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
    mSides[IRIGHT] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
    mSides[ITOP] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
    mSides[IBOTTOM] = static_cast<AccessorDirection>(AccessorDirection::in | AccessorDirection::out);
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

