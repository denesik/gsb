#pragma once
#ifndef Agent_h_
#define Agent_h_

#include "../TemplateFactory.h"
#include <tools/Common.h>
#include <memory>
#include "IGui.h"
#include <array>
#include "../tools/CoordSystem.h"
#include <rapidjson/fwd.h>
#include "../fwd.h"

namespace Magnum {
  class Timeline;
}

using AccessorId = unsigned int;

//TODO: ���������� ����������.
enum /*class*/ AccessorDirection
{
  none = 0,
  in = 1 << 0,
  out = 1 << 1,
};

// ��������. ����� ������� �� ������ ����, ������� �������� ��������� �����, ���������� ����� ����������� ������� ������.
// ������ ��������� ��������� ����� ���. ��� ������ ���� ��������� ������ �����.
class GSB_NOVTABLE Accessor : public NoContextGui
{
public:
  using factory = TemplateFactory<std::string, Accessor, void(const DataBase &, const rapidjson::Value &, Block &)>;

  Accessor() = delete;
  virtual ~Accessor() = default;

  Accessor(const Accessor &other);
  Accessor(Accessor &&other);
  /// �� ���������� ��������� ����������� � �����������.
  const Accessor &operator=(const Accessor &other) = delete;
  Accessor &operator=(Accessor &&other) = delete;

  /// ������������ ��� ������������.
  Accessor(const Accessor &other, Block &parent);
  Accessor(Accessor &&other, Block &parent);

  /// ������� ������� ����� �������.
  Accessor(const DataBase &db, const rapidjson::Value &val, Block &parent);

  /// ��� ������������ ��������� ������ �������� �����.
  virtual std::unique_ptr<Accessor> Clone(Block &parent) const = 0;

  /// rtti
  virtual AccessorId Id() const = 0;
  
  Block &Parent() const;

  // �������� ����������� �� ��������� �������.
  AccessorDirection GetDirection(SideIndex side);

  AccessorId Name() const;

  static AccessorId NameFromString(const std::string &name);

protected:
  Block &mParent;
  const AccessorId m_name;
  std::array<AccessorDirection, 6> mSides;

private:
  AccessorId LoadName(const rapidjson::Value &val);

};

//TODO: ������� ������ ������������ �������
//http://stackoverflow.com/questions/15858141/conveniently-declaring-compile-time-strings-in-c/15863804#15863804
template <class Base, AccessorId id>
class NumeredAgent : public Accessor
{
public:
  NumeredAgent() = delete;
  virtual ~NumeredAgent() = default;

  NumeredAgent(const NumeredAgent &other)
    : Accessor(other)
  {}
  NumeredAgent(NumeredAgent &&other)
    : Accessor(std::move(other))
  {}

  /// �� ���������� ��������� ����������� � �����������.
  const NumeredAgent &operator=(const NumeredAgent &other) = delete;
  NumeredAgent &operator=(NumeredAgent &&other) = delete;

  /// ������������ ��� ������������.
  NumeredAgent(const NumeredAgent &other, Block &parent)
    : Accessor(other, parent)
  {}
  NumeredAgent(NumeredAgent &&other, Block &parent)
    : Accessor(std::move(other), parent)
  {}

  /// ������� ������� ����� �������.
  NumeredAgent(const DataBase &db, const rapidjson::Value &val, Block &parent)
    : Accessor(db, val, parent)
  {}

  std::unique_ptr<Accessor> Clone(Block &parent) const override
  {
    return std::make_unique<Base>(static_cast<const Base &>(*this), parent);
  }
  
  AccessorId Id() const override { return id; }
  static constexpr AccessorId TypeId() { return id; }
};


#endif // Agent_h_
