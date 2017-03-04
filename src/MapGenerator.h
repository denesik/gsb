#pragma once
#include "IMapGenerator.h"

class PrimitivaMountains : IMapGenerator
{
public:
  PrimitivaMountains(const BlocksDataBase &db, float power);
  void Generate(Sector &sector, std::array<BlockId, SECTOR_CAPACITY> &blocks) override;

private:
  float mPower;
};
