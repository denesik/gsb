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
class TemplateFactory3;

template <class ID, class Base, class ...Args>
class TemplateFactory3<ID, Base, void(Args...)>
{
private:
  using Id = ID;
  using ElementType = std::unique_ptr<Base>;
  using Creator = std::function<ElementType(Args &&...)>;
  using Container = std::map<Id, Creator>;
  using FactoryType = TemplateFactory3<Id, Base, void(Args...)>;
public:

  static ElementType create(const Id &id, Args &&...args)
  {
    const auto &it = container.find(id);
    return (it != container.end()) ? (it->second)(std::forward<Args>(args)...) : nullptr;
  }

private:
  static Container container;
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
  using FactoryType = TemplateFactory3<Id, Base, void(Args...)>;
public:

  static bool add(const Id &id)
  {
    Creator func = [](Args &&...args) -> std::unique_ptr<Element>
    {
      return std::make_unique<Element>(std::forward<Args>(args)...);
    };

    const auto &it = FactoryType::container.find(id);
    if (it == FactoryType::container.end())
    {
      LOG(trace) << "class id = \"" << id << "\" register";
      FactoryType::container.insert(Container::value_type(id, func));
    }
    else
    {
      LOG(trace) << "class id = \"" << id << "\" override with message: ";
      it->second = func;
    }

    return true;
  }
};


////////////////////////////////
////////////////////////////////
////////////////////////////////
////////////////////////////////
////////////////////////////////


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

//**************************
//**************************
//**************************


// template <class IdType, class FuncType>
// class TemplateFactory2;

template <class IdType, class Base>
class TemplateFactoryAny : boost::noncopyable
{
public:
  using IdTypeUsing = IdType;
protected:
  using BasePtr = std::unique_ptr<Base>;
  using CreateFunc = std::function<BasePtr(const boost::any &val)>;
  using FactoryMap = std::map<IdType, CreateFunc>;

public:
  template<class... Args>
  BasePtr Create(const IdType & id, const Args &...args) const
  {
    typename FactoryMap::const_iterator it = map_.find(id);

    return (it != map_.end()) ? (it->second)(boost::any(std::tuple<const Args &...>(args...))) : nullptr;
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


template<class... Args>
struct TypeSaver
{

};


template <class T>
class RegisterElementAny
{
public:
  template <class Factory, class... Args>
  RegisterElementAny(Factory & factory, const typename Factory::IdTypeUsing & id, TypeSaver<Args...>)
  {
    using Tuple = std::tuple<const Args &...>;
    call_func<Tuple>(factory, id, std::index_sequence_for<Args...>{});
  }

private:
  template<class Tuple, class Factory, std::size_t ...I>
  void call_func(Factory & factory, const typename Factory::IdTypeUsing & id, std::index_sequence<I...>)
  {
    factory.Add(id, [](const boost::any &val) -> std::unique_ptr<T>
    {
      LOG(trace) << "------" << boost::typeindex::type_id<Tuple>().name();
      LOG(trace) << "------" << val.type().name();

      auto tuple = boost::any_cast<Tuple>(val); 
      return std::make_unique<T>(std::get<I>(tuple)...);
    });
  }
};



#define REGISTER_ELEMENT_ANY(type, factory, id, ...) \
namespace                                           \
{                                                   \
RegisterElementAny<type> RegisterElementAny##type(factory, id, TypeSaver<__VA_ARGS__>());  \
}



#endif // AGENTFACTORY_H