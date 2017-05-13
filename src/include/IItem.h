#pragma once
#include <tools/Common.h>
#include <boost/noncopyable.hpp>
#include "TemplateFactory.h"
#include "IJsonSerializable.h"
#include <mapgen_types.h>

// TODO: переименовать в элемент
class GSB_NOVTABLE IItem : public IJsonSerializable
{
public:
  using factory = TemplateFactory<std::string, IItem, void()>;

  virtual ~IItem() = default;

  const std::string & GetName() const;
  const std::string & GetDescription() const;
  void SetName(const std::string & name);
  void SetDescription(const std::string & descriprion);

  void SetBlock(BlockId b);

  BlockId GetBlock() const;

protected:
  std::string mName, mDescription;
  BlockId mBlock = 0;
};

