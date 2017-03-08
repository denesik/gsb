#include "Agent.h"
#include <Magnum/Timeline.h>
#include "../../imgui/imgui.h"

void Agent::DrawGui(const Magnum::Timeline &dt)
{
}

BlockDynamicPart* Agent::Parent() const
{
  return mParent;
}

AgentFactory::FactoryType& AgentFactory::Get()
{
  static FactoryType factory;
  return factory;
}
