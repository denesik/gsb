#pragma once
#ifndef BlockStaticPart_h__
#define BlockStaticPart_h__

#include <stdint.h>
#include <memory>
#include "Tesselator.h"
#include <boost\optional\optional.hpp>
#include <include\mapgen_types.h>
#include <tools\Common.h>

class Model;
class DataBase;

// BlockId == 0 - ������ ���� / ���� ����������� / ���� �� ��������.

// �������� ����������� ������ ��� �����.
// ��������, ���������� � �.�.

// TODO: �����������!!!
class StaticBlock
{
public:
  StaticBlock();
  ~StaticBlock();

  void SetTesselator(std::unique_ptr<Tesselator> tesselator);

  const std::unique_ptr<Tesselator> &GetTesselator() const;

  void TestAddModel(const DataBase &db);

  boost::optional<const Model &> GetModel() const;

  const std::vector<ItemId> & GetDropList();
  void SetDropList(const std::vector<ItemId> &);

private:
  std::unique_ptr<Tesselator> mTesselator;
  std::unique_ptr<Model> mModel;
  std::vector<ItemId> mDrop;
};



#endif // BlockStaticPart_h__
