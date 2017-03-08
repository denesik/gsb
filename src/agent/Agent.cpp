#include "Agent.h"
#include <Magnum/Timeline.h>
#include "../../imgui/imgui.h"

void Agent::JsonLoad(const rapidjson::Value& val)
{
}

void Agent::DrawGui(Magnum::Timeline dt)
{
}

GameObject* Agent::Parent() const
{
  return mParent;
}

AgentFactory::FactoryType& AgentFactory::Get()
{
  static FactoryType factory;
  return factory;
}
