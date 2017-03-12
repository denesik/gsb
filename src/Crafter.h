#pragma once
#ifndef Crafter_h__
#define Crafter_h__

#include <Magnum/Magnum.h>
#include "IRecipe.h"
#include "Timer.h"
#include "agent/agents/AccessorItem.h"
#include <memory>

class DataBase;

// TODO: ������� ��� ����� ����� ���������.
// TODO: �������� �����������
class Crafter
{
public:
  // TODO: ���������� �� unique ptr
  Crafter(std::unique_ptr<IRecipe> recipe, bool fast_components = false);

  void Update(const Magnum::Timeline &dt, const DataBase &db);
  void SetInput(Accessor &agent);
  void SetOutput(Accessor &agent);

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
  AccessorItem *mInput;
  AccessorItem *mOutput;
  std::unique_ptr<IRecipe> m_recipe_type;

  const IRecipe *m_current_recipe = nullptr;
  bool m_runned = false;

  float m_time = 0.0f;

  // ���������� ������������ � ������ ������ ��� � �����?
  bool m_fast_components = false;
};



#endif // Crafter_h__
