#pragma once
#include <tools/Common.h>
#include <boost/noncopyable.hpp>
#include "TemplateFactory.h"
#include "IJsonSerializable.h"

// TODO: переименовать в элемент
class GSB_NOVTABLE IItem GSB_ABSTRACT : public IJsonSerializable
{
public:
  using factory = TemplateFactory<std::string, IItem, void()>;

  virtual ~IItem() = default;

  const std::string & GetName() const;
  const std::string & GetDescription() const;
  void SetName(const std::string & name);
  void SetDescription(const std::string & descriprion);

private:
  std::string mName, mDescription;
};

