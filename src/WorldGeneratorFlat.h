#pragma once
#include <IMapGenerator.h>
#include "../FastNoise/FastNoise.h"

class WorldGeneratorFlat : public IMapGenerator
{
public:
  // Inherited via IMapGenerator
  Layering GetLayering(const DataBase & db, int x, int z) const override;
  unsigned short GetGroundLevel(const DataBase & db, int x, int z) const override;
  const std::string & GetBiome(const DataBase & db, int x, int z) const override;
};

class WorldGeneratorFlat2 : public IMapGenerator
{
public:
  // Inherited via IMapGenerator
  Layering GetLayering(const DataBase & db, int x, int z) const override;
  unsigned short GetGroundLevel(const DataBase & db, int x, int z) const override;
  const std::string & GetBiome(const DataBase & db, int x, int z) const override;

  WorldGeneratorFlat2(int seed = 1234);

private:
  unsigned short water_level = 80;
  unsigned short land_level = 47;
  float hill_multiplier = 33.f;
  float freq = 0.01f;
  std::string dirt_analog = "dirt";
  std::string grass_analog = "grass";

  mutable FastNoise noise;
};