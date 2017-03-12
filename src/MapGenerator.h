#pragma once
#include "IMapGenerator.h"

class PrimitivaMountains : public IMapGenerator
{
public:
  PrimitivaMountains(const DataBase &db, float power);
  void Generate(Sector &sector) override;

private:
  float mPower;
};
