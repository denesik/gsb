#include "BiomeGenerator.h"

gsb::BiomeGenerator::BiomeGenerator()
{
}

void gsb::BiomeGenerator::Generate(Sector & sector) const
{
}

gsb::interval_set<unsigned int, BlockId> gsb::BiomeGenerator::GetLayering(const DataBase &db, int x, int z)
{
  gsb::interval_set<unsigned int, BlockId> set;
  set.insert(block_range{ 50,{ 40, 1 } });
  set.insert(block_range{ 39,{ 30, 2 } });
  set.insert(block_range{ 29,{ 20, 3 } });
  set.insert(block_range{ 19,{ 5, 4 } });
  set.insert(block_range{ 4,{ 1, 5 } });
  set.insert(block_range{ 0,{ 0, 6 } });

  return set;
}

unsigned short gsb::BiomeGenerator::GetGroundLevel(const DataBase &db, int x, int z)
{
  return 50;
}

const std::string & gsb::BiomeGenerator::GetBiome(const DataBase &db, int x, int z)
{
  return "some biome";
}
