#include "ConfiguratableMapGenerator.h"
#include <Sector.h>

ConfiguratableMapGenerator::ConfiguratableMapGenerator(const DataBase & db, int seed) : IMapGenerator(db)
{
  noise.SetSeed(seed);
  noise.SetNoiseType(FastNoise::Simplex);

  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::InputInt("dirt_level", &dirt_level); });
  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::InputFloat("hill_multiplier", &hill_multiplier); });
}

void ConfiguratableMapGenerator::Generate(Sector & sector)
{
  BlockId air = 0;
  BlockId dirt = m_Db.BlockIdFromName("dirt").value_or(0);
  BlockId grass = m_Db.BlockIdFromName("grass").value_or(0);
  //BlockId furnance = m_Db.BlockIdFromName("furnance").value_or(0);

  const auto &sec_pos = sector.GetPos();
  for (auto i = 0; i < SECTOR_SIZE; ++i)
    for (auto j = 0; j < SECTOR_SIZE; ++j)
    {
      const auto &wb_pos = cs::SBtoWB({ i, j, 0 }, sec_pos);
      auto value = noise.GetNoise(wb_pos.x(), wb_pos.z()) / 2.f + 1.f;

      for (auto k = 0; k < SECTOR_SIZE; ++k)
      {
        auto sbpos = SBPos(i, j, k);

        if (wb_pos.y() < dirt_level + value * hill_multiplier)
        {
          sector.CreateBlock(sbpos, dirt);
        }
        else if (wb_pos.y() > dirt_level + static_cast<int>(value * hill_multiplier) && wb_pos.y() < dirt_level + static_cast<int>(value * hill_multiplier) + 1.1)
        {
          sector.CreateBlock(sbpos, grass);
        }
        else
        {
          sector.CreateBlock(sbpos, air);
        }
      }
    }
}