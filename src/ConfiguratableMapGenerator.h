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
  void Generate(Sector & sector) override;

  int dirt_level = 0;
  float hill_multiplier = 35.f;

protected:
  int seed;

private:
  FastNoise noise;
};