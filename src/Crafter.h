#pragma once
#ifndef Crafter_h__
#define Crafter_h__

#include <Magnum/Magnum.h>
#include "IRecipe.h"
#include "Timer.h"
#include "accessors/AccessorItem.h"
#include <memory>
#include "TemplateFactory.h"

class DataBase;

// TODO: ������� ��� ����� ����� ���������.
// TODO: �������� �����������
// �������� ������ ������ ������ ������������� ��������� (����� �����) � �� ����� json.
// ������� �������. ������� ����� �� ����� ��������� � ������ ��������.
class Crafter
{
public:
  Crafter() = delete;

  Crafter(std::unique_ptr<IRecipe> recipe, bool fast, AccessorItem * a = nullptr, AccessorItem * b = nullptr);


  Crafter(bool fast, AccessorItem * input = nullptr, AccessorItem * output = nullptr)
    : m_fast_components(fast), mInput(input), mOutput(output) {};

  Crafter(const Crafter &other);

  void Update(const Magnum::Timeline &dt, const DataBase &db);
  void SetInput(Accessor &accessor);
  void SetOutput(Accessor &accessor);

  float Progress() const; // TODO int?

  // ��������� �����.
  void Run();

  // ���������� �����.
  void Stop();

  // ������� �����?
  bool Runned() const;

  // ����� ��������� �����?
  bool Ready() const;

private:
  AccessorItem *mInput = nullptr;
  AccessorItem *mOutput = nullptr;

  // TODO_Recipe 
  // ���� ������� �� ���� �������.
  std::unique_ptr<IRecipe> m_recipe_type;

  const IRecipe *m_current_recipe = nullptr;
  bool m_runned = false;

  float m_time = 0.0f;

  // ���������� ������������ � ������ ������ ��� � �����?
  bool m_fast_components = false;

private:
};




//#define REGISTER_CRAFTER_CLASS(type, ...) REGISTER_ELEMENT2(type, CrafterFactory::Get(), #type, __VA_ARGS__)

struct CrafterFactory : boost::noncopyable
{
  using FactoryType = TemplateFactory2<std::string, Crafter>;
  static FactoryType &Get()
  {
    static FactoryType factory;
    return factory;
  }
};

//REGISTER_CRAFTER_CLASS(Crafter, int);


namespace                                           
{                                                   
  RegisterElement2<Crafter> RegisterElement2Crafter(CrafterFactory::Get(), "Crafter", std::tuple<bool, AccessorItem *, AccessorItem* >());
}

#endif // Crafter_h__
