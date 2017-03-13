#pragma once
#ifndef Agent_h_
#define Agent_h_

#include "../TemplateFactory.h"
#include <tools/Common.h>
#include <boost/noncopyable.hpp>
#include <rapidjson/document.h>
#include <memory>
#include "IJsonSerializable.h"
#include "IGui.h"
#include <array>
#include "../tools/CoordSystem.h"

namespace Magnum {
  class Timeline;
}

class Block;

using AccessorId = unsigned int;

//TODO: ѕерегрузка операторов.
enum /*class*/ AccessorDirection
{
  none = 0,
  in = 1 << 0,
  out = 1 << 1,
};

class GSB_NOVTABLE Accessor : public IGui
{
public:
  using factory = TemplateFactory3<std::string, Accessor, void()>;

  Accessor() = default;
  virtual ~Accessor() = default;

  virtual std::unique_ptr<Accessor> Clone(Block *parent) const = 0;

  virtual AccessorId Id() const = 0;
  
  void DrawGui(const Magnum::Timeline &dt) override;
  
  virtual void JsonLoad(const DataBase & db, const rapidjson::Value &val);

  Block *Parent() const;

  // ѕолучить направление на указанной стороне.
  AccessorDirection GetDirection(SideIndex side);

protected:
  Block *mParent = nullptr;
  
  std::array<AccessorDirection, 6> mSides;
};

//TODO: сделать модную инстанциацию строкой
//http://stackoverflow.com/questions/15858141/conveniently-declaring-compile-time-strings-in-c/15863804#15863804
template <class Base, AccessorId aId>
class NumeredAgent : public Accessor
{
public:
  static constexpr AccessorId TypeId() { return aId; }
  std::unique_ptr<Accessor> Clone(Block *parent) const override
  {
    auto t = std::make_unique<Base>(*static_cast<const Base *>(this));
    t->mSides = mSides;
    t->mParent = parent; // TODO  ривовато, надо бы перенести в конструктор.
    return t;
  }
    
  AccessorId Id() const override { return aId; }
};


#endif // Agent_h_
