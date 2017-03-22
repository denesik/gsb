#include "ConfiguratableMapGenerator.h"
#include <Sector.h>

ConfiguratableMapGenerator::ConfiguratableMapGenerator(const DataBase & db, MapGenConfiguration c, int seed) : IMapGenerator(db), conf(c)
{
  noise.SetSeed(seed);
  noise.SetNoiseType(FastNoise::Perlin);
  //noise.SetCellularDistanceFunction(FastNoise::Natural);
  //noise.SetCellularReturnType(FastNoise::NoiseLookup);

  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::InputInt("water_level", &conf.water_level); });
  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::InputInt("land_level", &conf.land_level); });
  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::DragFloat("hill_multiplier", &conf.hill_multiplier); });
  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::DragFloat("frequency", &conf.freq); noise.SetFrequency(conf.freq); });
}

void ConfiguratableMapGenerator::Generate(Sector & sector) const
{
  BlockId air = 0;
  BlockId dirt = m_Db.BlockIdFromName(conf.dirt_analog).value_or(0);
  BlockId grass = m_Db.BlockIdFromName(conf.grass_analog).value_or(0);
  BlockId water = m_Db.BlockIdFromName("water").value_or(0);
  BlockId furnance = m_Db.BlockIdFromName("furnance").value_or(0);
  BlockId stone = m_Db.BlockIdFromName("stone").value_or(0);

  const auto &sec_pos = sector.GetPos();
  for (auto i = 0; i < SECTOR_SIZE; ++i)
    for (auto k = 0; k < SECTOR_SIZE; ++k)
    {
      const auto &wb_pos = cs::SBtoWB({ i, 0, k }, sec_pos);
      auto value = static_cast<float>(noise.GetNoise(static_cast<float>(wb_pos.x()), static_cast<float>(wb_pos.z()))) / 2.f + 1.f;

	  int hill_level = static_cast<int>(value * conf.hill_multiplier + conf.land_level);

	  
	  for (auto j = 0; j < std::min(conf.land_level, int(SECTOR_HEIGHT)); ++j)
	  {
	    auto sbpos = SBPos(i, j, k);
	    sector.CreateBlock(sbpos, stone);
	  }

	  for (auto j = conf.land_level; j < std::min(hill_level, int(SECTOR_HEIGHT)); ++j)
	  {
		  auto sbpos = SBPos(i, j, k);
		  sector.CreateBlock(sbpos, dirt);
	  }

	  for (auto j = std::min(hill_level, int(SECTOR_HEIGHT)); j < std::min(hill_level + 1, int(SECTOR_HEIGHT)); ++j)
	  {
		  auto sbpos = SBPos(i, j, k);
		  sector.CreateBlock(sbpos, grass);
	  }

	  for (auto j = std::min(hill_level + 1, int(SECTOR_HEIGHT)); j < std::min(conf.water_level, int(SECTOR_HEIGHT)); ++j)
	  {
		  auto sbpos = SBPos(i, j, k);
		  sector.CreateBlock(sbpos, water);
	  }

	  for (auto j = std::min(std::max(hill_level + 1, conf.water_level), int(SECTOR_HEIGHT)); j < SECTOR_HEIGHT; ++j)
	  {
		  auto sbpos = SBPos(i, j, k);
		  sector.CreateBlock(sbpos, air);
	  }

	  if (rand() % 50 == 0)
	  {
		  auto sbpos = SBPos(i, hill_level + 1, k);
		  sector.CreateBlock(sbpos, furnance);
	  }
    }
}

MapGenConfiguration::MapGenConfiguration(const std::string & d, const std::string & g, float m) : dirt_analog(d), grass_analog(g), hill_multiplier(m)
{
}
