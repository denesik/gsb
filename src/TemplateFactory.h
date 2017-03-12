/*******************************************************************************
Copyright (C) 2016 Samsonov Andrey

This software is distributed freely under the terms of the MIT LICENSE.
See "LICENSE.txt"
*******************************************************************************/

#include "Log.h"
#ifndef AGENTFACTORY_H
#define AGENTFACTORY_H

#include <map>
#include <memory>
#include <functional>
#include <boost/noncopyable.hpp>
#include "Log.h"

template <class IdType, class Base, class ... Args>
class TemplateFactory : boost::noncopyable
{
public:
  using IdTypeUsing = IdType;
protected:
  using BasePtr = std::unique_ptr<Base>;
  using CreateFunc = std::function<BasePtr(Args...)>;
  using FactoryMap = std::map<IdType, CreateFunc>;

public:
  BasePtr Create(const IdType & id, Args && ... args) const
  {
    typename FactoryMap::const_iterator it = map_.find(id);
    return (it != map_.end()) ? (it->second)(std::forward<Args>(args)...) : BasePtr(std::forward<Args>(args)...);
  }

  void Add(const IdType & id, CreateFunc func, const std::string comment = "")
  {
    auto i = map_.find(id);
    if (i == map_.end())
    {
      LOG(trace) << "agent class id = \"" << id << "\" register";
      map_.insert(FactoryMap::value_type(id, func));
    }
    else
    {
      if (!comment.empty())
        LOG(trace) << "agent class id = \"" << id << "\" override with message: " << comment;
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

#endif // AGENTFACTORY_H