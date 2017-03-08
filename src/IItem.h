#pragma once
#include <tools/Common.h>
#include <boost/noncopyable.hpp>
#include "TemplateFactory.h"
#include "IJsonSerializable.h"

class GSB_NOVTABLE IItem : public IJsonSerializable
{
public:
  virtual ~IItem() = default;
};

#define REGISTER_ITEM_CLASS(type) REGISTER_ELEMENT(type, ItemFactory::Get(), #type)

struct ItemFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory<std::string, IItem>;
  static FactoryType &Get()
  {
    static FactoryType factory;
    return factory;
  }
};