#pragma once
#ifndef Agent_h_
#define Agent_h_

#include "../TemplateFactory.h"
#include <boost/noncopyable.hpp>
#include <rapidjson/document.h>
#include <memory>

class GameObject;

using PAgent = std::shared_ptr<class Agent>;

using AId = unsigned int;

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

  virtual PAgent Clone(GameObject *parent) = 0;

  virtual bool IsBasedOn(const AId & type) = 0;

  virtual void JsonLoad(const rapidjson::Value &val);

  GameObject* Parent() const;

protected:
  GameObject *mParent;
};

template <AId aId>
class NumeredAgent : public Agent
{
public:
  static constexpr AId TypeId() { return aId; }
};

#define REGISTER_AGENT(type) REGISTER_ELEMENT(type, AgentFactory::Get(), StringIntern(#type))
#define AGENT(type) virtual std::string GetName() const override { return #type; } \
					static std::string TypeName() { return #type; } \
				  virtual bool IsBasedOn(const Aid & aid) { return type::TypeId() == aid; }

#define AGENT_EX(type, base) virtual std::string GetName() const override { return #type; } \
					static std::string TypeName() { return #type; } \
				  virtual bool IsBasedOn(const Aid & aid) { if(type::TypeId() == aid) return true; return base::IsBasedOn(type); }

struct AgentFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory<std::string, Agent>;
  static FactoryType &Get();
};

#endif // Agent_h_
