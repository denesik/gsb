#pragma once
#include <IMapGenerator.h>
#include <IGui.h>
#include "../FastNoise/FastNoise.h"
#include <memory>

struct MapGenConfiguration
{
	MapGenConfiguration(const std::string &d, const std::string &g, float mult = 33.f);

	int water_level = 80;
	int land_level = 47;
	float hill_multiplier = 33.f;
	float freq = 0.01f;
	std::string dirt_analog = "dirt";
	std::string grass_analog = "grass";
};

class ConfiguratableMapGenerator : public IMapGenerator
{
public:
  ConfiguratableMapGenerator(const DataBase & db, MapGenConfiguration c, int seed = 0);

  // Inherited via IMapGenerator
  void Generate(Sector & sector) const override;

  MapGenConfiguration conf;

protected:
  int seed;

private:
  mutable FastNoise noise;
};