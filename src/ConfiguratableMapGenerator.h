#pragma one
#include <IMapGenerator.h>
#include <IGui.h>
#include "../FastNoise/FastNoise.h"
#include <memory>

class ConfiguratableMapGenerator : public IMapGenerator
{
public:
  ConfiguratableMapGenerator(const DataBase & db, int seed = 0);

  // Inherited via IMapGenerator
  void Generate(Sector & sector) const override;

  int water_level = 40;
  int land_level = 7;
  float hill_multiplier = 33.f;
  float freq = 0.05;

protected:
  int seed;

private:
  mutable FastNoise noise;
};