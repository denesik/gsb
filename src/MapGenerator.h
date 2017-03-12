#pragma once
#include "IMapGenerator.h"

class PrimitivaMountains : public IMapGenerator
{
public:
  PrimitivaMountains(const BlocksDataBase &db, float power);

private:
  float flatness(float tx, float ty);
  float dens(float tx, float ty, float tz);
  bool is_cluster(float tx, float ty, float tz, float type, float prob);
  bool solid(float tx, float ty, float tz);
  void Generate(Sector &sector) override;

  float mPower;

  float mTopDownscaler = 100.f;
  float mBotDownscaler = 100.f;
};
