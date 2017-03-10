#include "Agent.h"
#include <Magnum/Timeline.h>
#include "../../imgui/imgui.h"


void Agent::DrawGui(const Magnum::Timeline &dt)
{
}

void Agent::JsonLoad(BlocksDataBase & db, const rapidjson::Value &val)
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
      if (directions[1] != '_') mSides[side] = static_cast<AgentDirection>(mSides[side] | AgentDirection::in);
      if (directions[2] != '_') mSides[side] = static_cast<AgentDirection>(mSides[side] | AgentDirection::out);
    }
  }
  else
  {
    mSides[IBACK] = static_cast<AgentDirection>(AgentDirection::in | AgentDirection::out);
    mSides[IFRONT] = static_cast<AgentDirection>(AgentDirection::in | AgentDirection::out);
    mSides[ILEFT] = static_cast<AgentDirection>(AgentDirection::in | AgentDirection::out);
    mSides[IRIGHT] = static_cast<AgentDirection>(AgentDirection::in | AgentDirection::out);
    mSides[ITOP] = static_cast<AgentDirection>(AgentDirection::in | AgentDirection::out);
    mSides[IBOTTOM] = static_cast<AgentDirection>(AgentDirection::in | AgentDirection::out);
  }
}

BlockDynamicPart* Agent::Parent() const
{
  return mParent;
}

AgentDirection Agent::GetDirection(SideIndex side)
{
  return mSides[side];
}

AgentFactory::FactoryType& AgentFactory::Get()
{
  static FactoryType factory;
  return factory;
}
