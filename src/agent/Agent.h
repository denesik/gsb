#pragma once
#ifndef Agent_h_
#define Agent_h_

#include "../TemplateFactory.h"
#include <boost/noncopyable.hpp>
#include <rapidjson/document.h>
#include <memory>

class GameObject;

using PAgent = std::unique_ptr<class Agent>;

using AId = unsigned int;

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

  virtual PAgent Clone(GameObject *parent) const = 0;
  virtual void JsonLoad(const rapidjson::Value &val);

  GameObject* Parent() const;

protected:
  GameObject *mParent;
};

template <class Base, AId aId>
class NumeredAgent : public Agent
{
public:
  static constexpr AId TypeId() { return aId; }
  PAgent Clone(GameObject* parent) const override
  {
    auto t = make_agent<Base>(*reinterpret_cast<const Base *>(this));
    t->mParent = parent;
    return t;
  }
};

#define REGISTER_AGENT(type) REGISTER_ELEMENT(type, AgentFactory::Get(), #type)
#define AGENT(type) virtual std::string GetName() const override { return #type; } \
					static std::string TypeName() { return #type; } \
				  virtual bool IsBasedOn(const AId & aid) override { return type::TypeId() == aid; } \
          static constexpr AId TypeId() { return aId; }

#define AGENT_EX(type, base) virtual std::string GetName() const override { return #type; } \
					static std::string TypeName() { return #type; } \
				  virtual bool IsBasedOn(const AId & aid) override { if(type::TypeId() == aid) return true; return base::IsBasedOn(type); } \
          static constexpr AId TypeId() { return aId; }

struct AgentFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory<std::string, Agent>;
  static FactoryType &Get();
};

#endif // Agent_h_
