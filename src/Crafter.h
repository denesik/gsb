#pragma once
#ifndef Crafter_h__
#define Crafter_h__

#include <Magnum/Magnum.h>
#include "IRecipe.h"
#include "Timer.h"
#include "agent/agents/Chest.h"
#include <memory>

class BlocksDataBase;

// TODO: ������� ��� ����� ����� ���������.
// TODO: �������� �����������
class Crafter
{
public:
  // TODO: ���������� �� unique ptr
  Crafter(std::unique_ptr<IRecipe> recipe);

  void Update(const Magnum::Timeline &dt, const BlocksDataBase &db);
  void SetInput(Agent &agent);
  void SetOutput(Agent &agent);

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
  Chest *mInput;
  Chest *mOutput;
  std::unique_ptr<IRecipe> m_recipe_type;

  const IRecipe *m_current_recipe = nullptr;
  bool m_runned = false;

  float m_time = 0.0f;

};



#endif // Crafter_h__
