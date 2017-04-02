#include "WorldGeneratorFlat.h"
#include "DataBase.h"
#include "Sector.h"



WorldGeneratorFlat::WorldGeneratorFlat(const DataBase &db)
  : IMapGenerator(db)
{
}

WorldGeneratorFlat::WorldGeneratorFlat(const DataBase &db, const std::string & id)
	: IMapGenerator(db), top_layer(id)
{
}


WorldGeneratorFlat::~WorldGeneratorFlat()
{
}

void WorldGeneratorFlat::Generate(Sector &sector) const
{
  const int dirt_level = 64;
  BlockId air = 0;
  BlockId dirt = m_Db.BlockIdFromName("dirt").value_or(air);
  BlockId grass = m_Db.BlockIdFromName(top_layer).value_or(air);
  BlockId furnance = m_Db.BlockIdFromName("furnance").value_or(air);
  BlockId number = m_Db.BlockIdFromName("number").value_or(air);
  BlockId world_side = m_Db.BlockIdFromName("world_side").value_or(air);

  const auto &sec_pos = sector.GetPos();

  for (auto i = 0; i < gSectorCapacity; ++i)
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
      if (wb_pos.y() == dirt_level + 1)
        if ((rand() % 100 == 1 || rand() % 100 == 2))
          sector.CreateBlock(sb_pos, world_side);
        else if (rand() % 100 == 1)
          ;//sector.CreateBlock(sb_pos, number);
        else
          sector.CreateBlock(sb_pos, air);
    }
  }
}
