#pragma once
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
class Generator
{
public:
  Generator() = delete;
  virtual ~Generator() = default;

  Generator(const Generator &other) = delete;
  Generator(Generator &&other) = delete;
  /// �� ���������� ��������� ����������� � �����������.
  const Generator &operator=(const Generator &other) = delete;
  Generator &operator=(Generator &&other) = delete;

  Generator(const Generator &other, Accessor &input, Accessor &output);
  Generator(Generator &&other, Accessor &input, Accessor &output);

  /// ������� ������� ����� �������.
  Generator(IRecipe::Tag tag, bool fast, Accessor &input, Accessor &output);

  void Update(const Magnum::Timeline &dt, const DataBase &db);

  float Progress() const; // TODO int?

                          // ��������� �����.
  void Run();

  // ���������� �����.
  void Stop();

  // ������� �����?
  bool IsRunned() const;

  // ����� ��������� �����?
  bool IsReady() const;

private:
  AccessorItem &mInput;
  AccessorItem &mOutput;

  IRecipe::Tag m_recipe_tag;

  boost::optional<const IRecipe &> m_current_recipe;
  bool m_runned = false;

  float m_time = 0.0f;

  // ���������� ������������ � ������ ������ ��� � �����?
  bool m_fast_components = false;

private:
};

