#pragma once
#include "IMapGenerator.h"

class PrimitivaMountains : public IMapGenerator
{
public:
  PrimitivaMountains(const DataBase &db, float power);

private:
  float flatness(float tx, float ty) const;
  float dens(float tx, float ty, float tz) const;
  bool is_cluster(float tx, float ty, float tz, float type, float prob) const;
  bool solid(float tx, float ty, float tz) const;
  void Generate(Sector &sector) const override;

  float mPower;

  float mTopDownscaler = 100.f;
  float mBotDownscaler = 100.f;
};
