/*******************************************************************************
Copyright (C) 2016 Samsonov Andrey

This software is distributed freely under the terms of the MIT LICENSE.
See "LICENSE.txt"
*******************************************************************************/

#ifndef AGENTFACTORY_H
#define AGENTFACTORY_H

#include "Log.h"
#include <map>
#include <memory>
#include <functional>
#include <boost/noncopyable.hpp>
#include "Log.h"

template <class IdType, class Base>
class TemplateFactory : boost::noncopyable
{
public:
  using IdTypeUsing = IdType;
protected:
  using BasePtr = std::unique_ptr<Base>;
  using CreateFunc = std::function<BasePtr()>;
  using FactoryMap = std::map<IdType, CreateFunc>;

public:
  BasePtr Create(const IdType & id) const
  {
    typename FactoryMap::const_iterator it = map_.find(id);
    return (it != map_.end()) ? (it->second)() : BasePtr();
  }

  void Add(const IdType & id, CreateFunc func, const std::string comment = "")
  {
    auto i = map_.find(id);
    if (i == map_.end())
    {
      LOG(trace) << "class id = \"" << id << "\" register";
      map_.insert(FactoryMap::value_type(id, func));
    }
    else
    {
      if (!comment.empty())
        LOG(trace) << "class id = \"" << id << "\" override with message: " << comment;
      i->second = func;
    }
  }

  FactoryMap map_;
};

template <class T>
class RegisterElement
{
public:
  template <class Factory>
  RegisterElement(Factory & factory, const typename Factory::IdTypeUsing & id)
  {
    factory.Add(id, []() -> std::unique_ptr<T> {
      return std::make_unique<T>();
    });
  }
};


#define REGISTER_ELEMENT(type, factory, id) \
namespace                                           \
{                                                   \
RegisterElement<type> RegisterElement##type(factory, id);  \
}

//*****************
//****************
// TODO: переделать на Base(Args)

template <class IdType, class Base, class Arg1, class Arg2>
class TemplateFactory1 : boost::noncopyable
{
public:
  using IdTypeUsing = IdType;
protected:
  using BasePtr = std::unique_ptr<Base>;
  using CreateFunc = std::function<BasePtr(const Arg1 &, const Arg2 &)>;
  using FactoryMap = std::map<IdType, CreateFunc>;

public:
  BasePtr Create(const IdType & id, const Arg1 &arg1, const Arg2 &arg2) const
  {
    typename FactoryMap::const_iterator it = map_.find(id);
    return (it != map_.end()) ? (it->second)(arg1, arg2) : std::make_unique<Base>(arg1, arg2);
  }

  void Add(const IdType & id, CreateFunc func, const std::string comment = "")
  {
    auto i = map_.find(id);
    if (i == map_.end())
    {
      LOG(trace) << "class id = \"" << id << "\" register";
      map_.insert(FactoryMap::value_type(id, func));
    }
    else
    {
      if (!comment.empty())
        LOG(trace) << "class id = \"" << id << "\" override with message: " << comment;
      i->second = func;
    }
  }

  FactoryMap map_;
};

template <class T, class Arg1, class Arg2>
class RegisterElement1
{
public:
  template <class Factory>
  RegisterElement1(Factory & factory, const typename Factory::IdTypeUsing & id)
  {
    factory.Add(id, [](const Arg1 &arg1, const Arg2 &arg2) -> std::unique_ptr<T> {
      return std::make_unique<T>(arg1, arg2);
    });
  }
};


#define REGISTER_ELEMENT1(type, arg1, arg2, factory, id) \
namespace                                           \
{                                                   \
RegisterElement1<type, arg1, arg2> RegisterElement1##type(factory, id);  \
}



#endif // AGENTFACTORY_H