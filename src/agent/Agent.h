#pragma once
#ifndef Agent_h_
#define Agent_h_

#include "../TemplateFactory.h"
#include <boost/noncopyable.hpp>
#include <rapidjson/document.h>
#include <memory>

class GameObject;

using PAgent = std::unique_ptr<class Agent>;

using AgentId = unsigned int;

template<class T, class... Args>
std::unique_ptr<T> make_agent(Args&&... args)
{
  return std::make_unique<T>(std::forward<Args>(args)...);
}

#ifdef _MSC_VER
#define AM_NOVTABLE __declspec(novtable)
#elif
#define AM_NOVTABLE /*__declspec(novtable)*/
#endif

class AM_NOVTABLE Agent
{
public:
  Agent() = default;
  virtual ~Agent() = default;

  virtual PAgent Clone() const = 0;
  virtual void JsonLoad(const rapidjson::Value &val);

  GameObject* Parent() const;

protected:
  GameObject *mParent;
};

template <class Base, AgentId aId>
class NumeredAgent : public Agent
{
public:
  static constexpr AgentId TypeId() { return aId; }
  PAgent Clone() const override
  {
    auto t = make_agent<Base>(*reinterpret_cast<const Base *>(this));
    return t;
  }
};

#define REGISTER_AGENT(type) REGISTER_ELEMENT(type, AgentFactory::Get(), #type)

struct AgentFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory<std::string, Agent>;
  static FactoryType &Get();
};

#endif // Agent_h_
