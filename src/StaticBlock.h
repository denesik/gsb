#pragma once
#ifndef BlockStaticPart_h__
#define BlockStaticPart_h__

#include <stdint.h>
#include <memory>
#include "Tesselator.h"
#include <boost\optional\optional.hpp>
#include <include\mapgen_types.h>

class Model;
class DataBase;

// BlockId == 0 - Пустой блок / блок отсутствует / блок не загружен.

// Содержит константные данные для блока.
// Материал, тесселятор и т.д.

// TODO: Рефакторинг!!!
class StaticBlock
{
public:
  StaticBlock();
  ~StaticBlock();

  void SetTesselator(std::unique_ptr<Tesselator> tesselator);

  const std::unique_ptr<Tesselator> &GetTesselator() const;

  void TestAddModel(const DataBase &db);

  boost::optional<const Model &> GetModel() const;

private:
  std::unique_ptr<Tesselator> mTesselator;
  std::unique_ptr<Model> mModel;
};



#endif // BlockStaticPart_h__
