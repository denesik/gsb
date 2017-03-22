#include "ConfiguratableMapGenerator.h"
#include <Sector.h>

ConfiguratableMapGenerator::ConfiguratableMapGenerator(const DataBase & db, int seed) : IMapGenerator(db)
{
  noise.SetSeed(seed);
  noise.SetNoiseType(FastNoise::Perlin);
  //noise.SetCellularDistanceFunction(FastNoise::Natural);
  //noise.SetCellularReturnType(FastNoise::NoiseLookup);

  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::InputInt("water_level", &water_level); });
  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::InputInt("land_level", &land_level); });
  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::DragFloat("hill_multiplier", &hill_multiplier); });
  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::DragFloat("frequency", &freq); noise.SetFrequency(freq); });
}

void ConfiguratableMapGenerator::Generate(Sector & sector) const
{
  BlockId air = 0;
  BlockId dirt = m_Db.BlockIdFromName("dirt").value_or(0);
  BlockId grass = m_Db.BlockIdFromName("grass").value_or(0);
  BlockId water = m_Db.BlockIdFromName("water").value_or(0);
  BlockId furnance = m_Db.BlockIdFromName("furnance").value_or(0);

  const auto &sec_pos = sector.GetPos();
  for (auto i = 0; i < SECTOR_SIZE; ++i)
    for (auto k = 0; k < SECTOR_SIZE; ++k)
    {
      const auto &wb_pos = cs::SBtoWB({ i, 0, k }, sec_pos);
      auto value = noise.GetNoise(wb_pos.x(), wb_pos.z()) / 2.f + 1.f;

	  int hill_level = value * hill_multiplier + land_level;

	  for (auto j = 0; j < std::min(hill_level, int(SECTOR_HEIGHT)); ++j)
	  {
		  auto sbpos = SBPos(i, j, k);
		  sector.CreateBlock(sbpos, dirt);
	  }

	  for (auto j = std::min(hill_level, int(SECTOR_HEIGHT)); j < std::min(hill_level + 1, int(SECTOR_HEIGHT)); ++j)
	  {
		  auto sbpos = SBPos(i, j, k);
		  sector.CreateBlock(sbpos, grass);
	  }

	  for (auto j = std::min(hill_level + 1, int(SECTOR_HEIGHT)); j < std::min(water_level, int(SECTOR_HEIGHT)); ++j)
	  {
		  auto sbpos = SBPos(i, j, k);
		  sector.CreateBlock(sbpos, water);
	  }

	  for (auto j = std::min(std::max(hill_level + 1, water_level), int(SECTOR_HEIGHT)); j < SECTOR_HEIGHT; ++j)
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