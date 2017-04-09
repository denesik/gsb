#include "WorldGeneratorFlat.h"
#include "DataBase.h"
#include <vector>
#include <limits>

Layering WorldGeneratorFlat::GetLayering(const DataBase & db, int x, int z) const
{
  unsigned short max = std::numeric_limits<unsigned short>::max();

  const int dirt_level = 64;
  BlockId air = 0;
  BlockId dirt = db.BlockIdFromName("dirt").value_or(air);
  BlockId grass = db.BlockIdFromName("grass").value_or(air);
  BlockId furnance = db.BlockIdFromName("furnance").value_or(air);
  BlockId number = db.BlockIdFromName("number").value_or(air);
  BlockId world_side = db.BlockIdFromName("world_side").value_or(air);

  Layering layering;
  layering += std::make_pair(boost::icl::interval<unsigned short>::right_open(0, 64), dirt);
  layering += std::make_pair(boost::icl::interval<unsigned short>::right_open(64, 65), grass);
  if ((rand() % 100 == 1 || rand() % 100 == 2))
  {
    layering += std::make_pair(boost::icl::interval<unsigned short>::right_open(65, 66), world_side);
    layering += std::make_pair(boost::icl::interval<unsigned short>::right_open(66, max), air);
  }
  else if (rand() % 100 == 1)
    ;//sector.CreateBlock(sb_pos, number);
  else
    layering += std::make_pair(boost::icl::interval<unsigned short>::right_open(65, max), air);

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
  unsigned short max = std::numeric_limits<unsigned short>::max();
  BlockId air = 0;

  if ((x > 0 && x < 10) || (z > 0 && z < 10))
    return Layering(std::make_pair(boost::icl::interval<unsigned short>::right_open(0, max), air));

  BlockId dirt = db.BlockIdFromName("dirt").value_or(0);
  BlockId dirt2 = db.BlockIdFromName("dirt2").value_or(0);
  BlockId dirt3 = db.BlockIdFromName("dirt3").value_or(0);
  BlockId dirt4 = db.BlockIdFromName("dirt4").value_or(0);
  BlockId grass = db.BlockIdFromName("grass").value_or(0);
  BlockId water = db.BlockIdFromName("water").value_or(0);
  BlockId furnance = db.BlockIdFromName("furnance").value_or(0);
  BlockId stone = db.BlockIdFromName("stone").value_or(0);

  const auto &wb_pos = SBPos{ x, 0, z };
  auto value = static_cast<float>(noise.GetNoise(static_cast<float>(wb_pos.x()), static_cast<float>(wb_pos.z()))) / 2.f + 1.f;

  auto sq = static_cast<float>(noise.GetNoise(static_cast<float>(-wb_pos.x()), static_cast<float>(-wb_pos.z()))) + 1.f;
  sq = std::pow(sq, 20);
  sq = std::max(std::logf(sq) / std::logf(10), 1.f);

  value = (value + value + sq) / 3.f;

  unsigned short hill_level = static_cast<unsigned short>(value * hill_multiplier + land_level);
  Layering layering;

  layering.insert(layering.begin(), std::make_pair(BlockInterval::right_open(0, land_level), stone));
  layering.insert(layering.begin(), std::make_pair(BlockInterval::right_open(land_level, hill_level - 15), dirt4));
  layering.insert(layering.begin(), std::make_pair(BlockInterval::right_open(land_level, hill_level - 9), dirt3));
  layering.insert(layering.begin(), std::make_pair(BlockInterval::right_open(land_level, hill_level - 4), dirt2));
  layering.insert(layering.begin(), std::make_pair(BlockInterval::right_open(land_level, hill_level), dirt));
  layering.insert(layering.begin(), std::make_pair(BlockInterval::right_open(hill_level, hill_level + 1), grass));
  layering.insert(layering.begin(), std::make_pair(BlockInterval::right_open(hill_level + 1, water_level), water));
  layering.insert(layering.begin(), std::make_pair(BlockInterval::right_open(std::max(static_cast<unsigned short>(hill_level + 1), water_level), max), air));

  return layering;
}

unsigned short WorldGeneratorFlat2::GetGroundLevel(const DataBase & db, int x, int z) const
{
  return 64;
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
