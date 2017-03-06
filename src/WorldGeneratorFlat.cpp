#include "WorldGeneratorFlat.h"
#include "BlocksDataBase.h"
#include "Sector.h"



WorldGeneratorFlat::WorldGeneratorFlat(const BlocksDataBase &db)
  : IMapGenerator(db)
{
}


WorldGeneratorFlat::~WorldGeneratorFlat()
{
}

void WorldGeneratorFlat::Generate(Sector &sector)
{
  const int dirt_level = 64;
  BlockId air = 0;
  BlockId dirt = m_Db.BlockIdFromName("dirt").value_or(0);
  BlockId grass = m_Db.BlockIdFromName("grass").value_or(0);

  const auto &sec_pos = sector.GetPos();

  for (auto i = 0; i < SECTOR_CAPACITY; ++i)
  {
    const auto &sb_pos = cs::BItoSB(i);
    const auto &wb_pos = cs::SBtoWB(sb_pos, sec_pos);

    if (wb_pos.y() < dirt_level)
    {
      sector.CreateBlock(sb_pos, dirt);
    }
    else if (wb_pos.y() == dirt_level)
    {
      sector.CreateBlock(sb_pos, grass);
    }
    else
    {
      sector.CreateBlock(sb_pos, air);
    }
  }
}
