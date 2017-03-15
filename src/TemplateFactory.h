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
#include <tuple>
#include <type_traits>
#include <boost/any.hpp>
#include <boost/type_index.hpp>




template <class ID, class Base, class Args>
class TemplateFactory;

template <class ID, class Base, class ...Args>
class TemplateFactory<ID, Base, void(Args...)>
{
private:
  using Id = ID;
  using ElementType = std::unique_ptr<Base>;
  using Creator = std::function<ElementType(Args...)>;
  using Container = std::map<Id, Creator>;
  using FactoryType = TemplateFactory<Id, Base, void(Args...)>;
public:

  template<class ...CArgs>
  static ElementType create(const Id &id, CArgs &&...args)
  {
    const auto &it = GetContainer().find(id);
    return (it != GetContainer().end()) ? (it->second)(std::forward<CArgs>(args)...) : nullptr;
  }

private:
  static Container &GetContainer()
  {
    static Container container;
    return container;
  };


  template <class Factory, class Type>
  friend class FactoryRegister;
public:
  template<class T>
  using Register = FactoryRegister<FactoryType, T>;
};

template <template<typename, typename, typename...> class Factory, typename ID, typename Base, typename Element, typename... Args>
class FactoryRegister<Factory<ID, Base, void(Args...)>, Element>
{
private:
  using Id = ID;
  using ElementType = std::unique_ptr<Base>;
  using Creator = std::function<ElementType(Args &&...)>;
  using Container = std::map<Id, Creator>;
  using FactoryType = TemplateFactory<Id, Base, void(Args...)>;
public:

  static bool add(const Id &id)
  {
    Creator func = [](Args &&...args) -> std::unique_ptr<Element>
    {
      return std::make_unique<Element>(std::forward<Args>(args)...);
    };

    const auto &it = FactoryType::GetContainer().find(id);
    if (it == FactoryType::GetContainer().end())
    {
      LOG(trace) << "class id = \"" << id << "\" register";
      FactoryType::GetContainer().insert(Container::value_type(id, func));
    }
    else
    {
      LOG(trace) << "class id = \"" << id << "\" override with message: ";
      it->second = func;
    }

    return true;
  }
};


#endif // AGENTFACTORY_H