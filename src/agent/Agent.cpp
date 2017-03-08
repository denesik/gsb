#include "Agent.h"

void Agent::JsonLoad(const rapidjson::Value& val)
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
