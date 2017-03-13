#pragma once
#include <tools/Common.h>
#include <boost/noncopyable.hpp>
#include "TemplateFactory.h"
#include "IJsonSerializable.h"

class GSB_NOVTABLE IItem : public IJsonSerializable
{
public:
  using factory = TemplateFactory<std::string, IItem, void()>;

  virtual ~IItem() = default;
};

