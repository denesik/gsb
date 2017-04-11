#include "WorldGeneratorFlat.h"
#include "DataBase.h"
#include <vector>
#include <limits>

#define ADDL layering.insert(layering.begin(), { BlockInterval::right_open(

Layering WorldGeneratorFlat::GetLayering(const DataBase & db, int x, int z) const
{
  const int dirt_level = 64;
  BlockId dirt = db.BlockIdFromName("dirt").value_or(0);
  BlockId grass = db.BlockIdFromName("grass").value_or(0);
  BlockId furnance = db.BlockIdFromName("furnance").value_or(0);
  BlockId number = db.BlockIdFromName("number").value_or(0);
  BlockId world_side = db.BlockIdFromName("world_side").value_or(0);

  Layering layering;
  layering += std::make_pair(boost::icl::interval<unsigned short>::right_open(0, 64), dirt);
  layering += std::make_pair(boost::icl::interval<unsigned short>::right_open(64, 65), grass);
  if ((rand() % 100 == 1 || rand() % 100 == 2))
    layering += std::make_pair(boost::icl::interval<unsigned short>::right_open(65, 66), world_side);

  return layering;
}

unsigned short WorldGeneratorFlat::GetGroundLevel(const DataBase & db, int x, int z) const
{
  return 64;
}

const std::string & WorldGeneratorFlat::GetBiome(const DataBase & db, int x, int z) const
{
  static const std::string b = "flat";
  return b;
}

Layering WorldGeneratorFlat2::GetLayering(const DataBase & db, int x, int z) const
{
  if ((x > 0 && x < 10) || (z > 0 && z < 10))
    return{};

  BlockId dirt = db.BlockIdFromName("dirt").value_or(0);
  BlockId dirt2 = db.BlockIdFromName("dirt2").value_or(0);
  BlockId dirt3 = db.BlockIdFromName("dirt3").value_or(0);
  BlockId dirt4 = db.BlockIdFromName("dirt4").value_or(0);
  BlockId grass = db.BlockIdFromName("grass").value_or(0);
  BlockId water = db.BlockIdFromName("water").value_or(0);
  BlockId furnance = db.BlockIdFromName("furnance").value_or(0);
  BlockId stone = db.BlockIdFromName("stone").value_or(0);

  unsigned short hill_level = GetGroundLevel(db,x,z);
  Layering layering;

  layering.insert(layering.begin(), { BlockInterval::right_open(0, land_level), stone });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level - 15), dirt4 });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level - 9), dirt3 });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level - 4), dirt2 });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level), dirt });
  layering.insert(layering.begin(), { BlockInterval::right_open(hill_level, hill_level + 1), grass });

  if(hill_level + 1 < water_level)
    layering.insert(layering.begin(), { BlockInterval::right_open(hill_level + 1, water_level), water });

  return layering;
}

unsigned short WorldGeneratorFlat2::GetGroundLevel(const DataBase & db, int x, int z) const
{
  auto value = static_cast<float>(noise.GetNoise(static_cast<float>(x), static_cast<float>(z))) / 2.f + 0.5f;

  auto sq = static_cast<float>(noise.GetNoise(static_cast<float>(-x), static_cast<float>(-z))) / 2.f + 0.5f;
  sq = std::pow(sq, 20);
  sq = std::max(std::logf(sq) / std::logf(10), 1.f);

  value = (value + value + sq) / 3.f;

  return static_cast<unsigned short>(value * hill_multiplier + land_level) + 1;
}

const std::string & WorldGeneratorFlat2::GetBiome(const DataBase & db, int x, int z) const
{
  static const std::string b = "flat";
  return b;
}

WorldGeneratorFlat2::WorldGeneratorFlat2(int seed)
{
  noise.SetSeed(seed);
  noise.SetNoiseType(FastNoise::SimplexFractal);
}

WorldGeneratorBiome::WorldGeneratorBiome(int seed)
  : flat2(seed)
{
  noise_dist.SetSeed(seed);
  noise_dist.SetNoiseType(FastNoise::Cellular);
  noise_dist.SetCellularReturnType(FastNoise::Distance);
  noise_dist.SetFrequency(0.02f);
  noise_dist.SetCellularDistanceFunction(FastNoise::Natural);

  noise.SetSeed(seed);
  noise.SetNoiseType(FastNoise::Cellular);
  noise.SetCellularReturnType(FastNoise::CellValue);
  noise.SetFrequency(0.02f);
  noise.SetCellularDistanceFunction(FastNoise::Natural);
}

Layering WorldGeneratorBiome::GetLayering(const DataBase & db, int x, int z) const
{
  return flat2.GetLayering(db, x, z);
}

float lerp(float t, float a, float b) {
  return (1 - t)*a + t*b;
}

unsigned short WorldGeneratorBiome::GetGroundLevel(const DataBase & db, int x, int z) const
{
  auto value = -static_cast<float>(noise_dist.GetNoise(static_cast<float>(x), static_cast<float>(z))) + 1.0f;
  value = std::min(value, 1.f);
  value = std::max(value, 0.f);
  return static_cast<unsigned short>(lerp(value, 40.f, flat2.GetGroundLevel(db,x,z)));
}

const std::string & WorldGeneratorBiome::GetBiome(const DataBase & db, int x, int z) const
{
  static const std::string b = "hz";
  return b;
}
