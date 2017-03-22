#include "WorldGeneratorTest.h"
#include "DataBase.h"
#include "Sector.h"
#include "TesselatorTexturedBlock.h"


WorldGeneratorTest::WorldGeneratorTest(const DataBase &db)
  : IMapGenerator(db)
{
}


WorldGeneratorTest::~WorldGeneratorTest()
{
}

void WorldGeneratorTest::Generate(Sector &sector) const
{
  BlockId air = 0;
  BlockId textured_block = m_Db.BlockIdFromName("textured_block").value_or(0);

  const auto &sec_pos = sector.GetPos();
  const SBPos min = { 0, 0, 0 };
  const SBPos max = { SECTOR_SIZE - 1, SECTOR_HEIGHT - 1, SECTOR_SIZE - 1 };

  for (auto i = 0; i < SECTOR_CAPACITY; ++i)
  {
    const auto &sb_pos = cs::BItoSB(i);
    
    // front:
    if (sb_pos.z() == min.z() && 
        sb_pos.y() != min.y() && sb_pos.y() != max.y() && 
        sb_pos.x() != min.x() && sb_pos.x() != max.x())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 0);
      data.set_texture(IRIGHT, 6);
      data.set_texture(IBACK, 6);
      data.set_texture(ILEFT, 6);
      data.set_texture(ITOP, 6);
      data.set_texture(IBOTTOM, 6);
      continue;
    }

    // back:
    if (sb_pos.z() == max.z() &&
      sb_pos.y() != min.y() && sb_pos.y() != max.y() &&
      sb_pos.x() != min.x() && sb_pos.x() != max.x())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 6);
      data.set_texture(IRIGHT, 6);
      data.set_texture(IBACK, 2);
      data.set_texture(ILEFT, 6);
      data.set_texture(ITOP, 6);
      data.set_texture(IBOTTOM, 6);
      continue;
    }

    // left:
    if (sb_pos.x() == max.x() &&
      sb_pos.y() != min.y() && sb_pos.y() != max.y() &&
      sb_pos.z() != min.z() && sb_pos.z() != max.z())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 6);
      data.set_texture(IRIGHT, 6);
      data.set_texture(IBACK, 6);
      data.set_texture(ILEFT, 3);
      data.set_texture(ITOP, 6);
      data.set_texture(IBOTTOM, 6);
      continue;
    }

    // right:
    if (sb_pos.x() == min.x() &&
      sb_pos.y() != min.y() && sb_pos.y() != max.y() &&
      sb_pos.z() != min.z() && sb_pos.z() != max.z())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 6);
      data.set_texture(IRIGHT, 1);
      data.set_texture(IBACK, 6);
      data.set_texture(ILEFT, 6);
      data.set_texture(ITOP, 6);
      data.set_texture(IBOTTOM, 6);
      continue;
    }

    // top:
    if (sb_pos.y() == max.y() &&
      sb_pos.x() != min.x() && sb_pos.x() != max.x() &&
      sb_pos.z() != min.z() && sb_pos.z() != max.z())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 6);
      data.set_texture(IRIGHT, 6);
      data.set_texture(IBACK, 6);
      data.set_texture(ILEFT, 6);
      data.set_texture(ITOP, 4);
      data.set_texture(IBOTTOM, 6);
      continue;
    }

    // down:
    if (sb_pos.y() == min.y() &&
      sb_pos.x() != min.x() && sb_pos.x() != max.x() &&
      sb_pos.z() != min.z() && sb_pos.z() != max.z())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 6);
      data.set_texture(IRIGHT, 6);
      data.set_texture(IBACK, 6);
      data.set_texture(ILEFT, 6);
      data.set_texture(ITOP, 6);
      data.set_texture(IBOTTOM, 5);
      continue;
    }

    // front right down
    if (sb_pos == min)
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 0);
      data.set_texture(IRIGHT, 1);
      data.set_texture(IBACK, 6);
      data.set_texture(ILEFT, 6);
      data.set_texture(ITOP, 6);
      data.set_texture(IBOTTOM, 5);
      continue;
    }

    // front left down
    if (sb_pos.x() == max.x() && sb_pos.y() == min.y() && sb_pos.z() == min.z())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 0);
      data.set_texture(IRIGHT, 6);
      data.set_texture(IBACK, 6);
      data.set_texture(ILEFT, 3);
      data.set_texture(ITOP, 6);
      data.set_texture(IBOTTOM, 5);
      continue;
    }

    // back left down
    if (sb_pos.x() == max.x() && sb_pos.y() == min.y() && sb_pos.z() == max.z())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 6);
      data.set_texture(IRIGHT, 6);
      data.set_texture(IBACK, 2);
      data.set_texture(ILEFT, 3);
      data.set_texture(ITOP, 6);
      data.set_texture(IBOTTOM, 5);
      continue;
    }

    // back right down
    if (sb_pos.x() == min.x() && sb_pos.y() == min.y() && sb_pos.z() == max.z())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 6);
      data.set_texture(IRIGHT, 1);
      data.set_texture(IBACK, 2);
      data.set_texture(ILEFT, 6);
      data.set_texture(ITOP, 6);
      data.set_texture(IBOTTOM, 5);
      continue;
    }

    // back left top
    if (sb_pos == max)
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 6);
      data.set_texture(IRIGHT, 6);
      data.set_texture(IBACK, 2);
      data.set_texture(ILEFT, 3);
      data.set_texture(ITOP, 4);
      data.set_texture(IBOTTOM, 6);
      continue;
    }

    // front left top
    if (sb_pos.x() == max.x() && sb_pos.y() == max.y() && sb_pos.z() == min.z())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 0);
      data.set_texture(IRIGHT, 6);
      data.set_texture(IBACK, 6);
      data.set_texture(ILEFT, 3);
      data.set_texture(ITOP, 4);
      data.set_texture(IBOTTOM, 6);
      continue;
    }

    // back right top
    if (sb_pos.x() == min.x() && sb_pos.y() == max.y() && sb_pos.z() == max.z())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 6);
      data.set_texture(IRIGHT, 1);
      data.set_texture(IBACK, 2);
      data.set_texture(ILEFT, 6);
      data.set_texture(ITOP, 4);
      data.set_texture(IBOTTOM, 6);
      continue;
    }

    // front right top
    if (sb_pos.x() == min.x() && sb_pos.y() == max.y() && sb_pos.z() == min.z())
    {
      sector.CreateBlock(sb_pos, textured_block);
      auto &data = TesselatorTexturedBlock::ToTexturedBlockData(*sector.GetTesselatorData(sb_pos));
      data.set_texture(IFRONT, 0);
      data.set_texture(IRIGHT, 1);
      data.set_texture(IBACK, 6);
      data.set_texture(ILEFT, 6);
      data.set_texture(ITOP, 4);
      data.set_texture(IBOTTOM, 6);
      continue;
    }

    sector.CreateBlock(sb_pos, air);
  }
}
