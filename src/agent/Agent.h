#pragma once
#ifndef Agent_h_
#define Agent_h_

#include "../TemplateFactory.h"
#include <tools/Common.h>
#include <boost/noncopyable.hpp>
#include <rapidjson/document.h>
#include <memory>
#include "IJsonSerializable.h"
#include "IGui.h"

namespace Magnum {
  class Timeline;
}

class BlockDynamicPart;

using AgentId = unsigned int;


class GSB_NOVTABLE Agent : public IJsonSerializable, public IGui
{
public:
  Agent() = default;
  virtual ~Agent() = default;

  virtual std::unique_ptr<Agent> Clone(BlockDynamicPart *parent) const = 0;
  virtual AgentId Id() const = 0;
  void DrawGui(const Magnum::Timeline &dt) override;

  BlockDynamicPart *Parent() const;

protected:
  BlockDynamicPart *mParent = nullptr;
};

//TODO: сделать модную инстанциацию строкой
//http://stackoverflow.com/questions/15858141/conveniently-declaring-compile-time-strings-in-c/15863804#15863804
template <class Base, AgentId aId>
class NumeredAgent : public Agent
{
public:
  static constexpr AgentId TypeId() { return aId; }
  std::unique_ptr<Agent> Clone(BlockDynamicPart *parent) const override
  {
    auto t = std::make_unique<Base>(*static_cast<const Base *>(this));
    t->mParent = parent;
    return t;
  }

  AgentId Id() const override { return aId; }
};

#define REGISTER_AGENT_CLASS(type) REGISTER_ELEMENT(type, AgentFactory::Get(), #type)

struct AgentFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory<std::string, Agent>;
  static FactoryType &Get();
};

#endif // Agent_h_
