#include "MapGenerator.h"
#include "PerlinNoise.h"
#include "Sector.h"
#include "TesselatorMicroBlock.h"

PrimitivaMountains::PrimitivaMountains(const BlocksDataBase &db, float power) : db(db), IMapGenerator(db), mPower(power)
{
}

#define GEN_OCT 5
float flatness(float tx, float ty)
{
  return (PerlinNoise2D(tx, ty, 2, 2, GEN_OCT) + 1) / 2.f;
}
float dens(float tx, float ty, float tz)
{
  float flat = 1 / 5.f;// flatness(tx / 1000.f, ty / 1000.f);
  if (ty < -SECTOR_SIZE)
    return 1;
  else if (ty < 0)
    return PerlinNoise3D(tx / 100.f, ty / 100.f, tz / 100.f, 1 + 5 * flat, 2, GEN_OCT) + ((-ty*(SECTOR_SIZE / 1000.f)));
  else
    return PerlinNoise3D(tx / 100.f, ty / 100.f, tz / 100.f, 1 + 5 * flat, 2, GEN_OCT) / ((ty + float(SECTOR_SIZE)) / float(SECTOR_SIZE));
}
float cluster(float tx, float ty, float tz)
{
  return PerlinNoise3D(tx / 3.f, ty / 3.f, tz / 3.f, 1 + 5, 2, 1);
}
// examples:
// 0.2 = nothing
// 0.3 = diamonds
// 0.31 = coal
// 0.4 = almost everything
// 1 = everything
bool is_cluster(float tx, float ty, float tz, float type, float prob)
{
  return cluster(tx + type * 3571, ty + type * 3557, tz + type * 3559) + 1 < prob * 2.f;
}
bool solid(float tx, float ty, float tz)
{
  return dens(tx, ty, tz + 16) > 0.1;
}
#undef GEN_OCT

void PrimitivaMountains::Generate(Sector &sector)
{
  auto sw = cs::StoW(sector.GetPos());
  for (int k = 0; k < SECTOR_SIZE; ++k)
  {
    for (int j = 0; j < SECTOR_SIZE; ++j)
    {
      for (int i = 0; i < SECTOR_SIZE; ++i)
      {
        if (solid(sw.x() + i, sw.y() + j, sw.z() + k))
        {
          if (!solid(sw.x() + i, sw.y() + j + 1, sw.z() + k))
          {
            sector.SetBlockId({ i,j,k }, 3);

            auto dyn = std::make_unique<BlockDinamicPart>();
            dyn->mTesselatorData = std::make_unique<TesselatorData>();
            auto &data = TesselatorMicroBlock::ToMicroblockData(*dyn->mTesselatorData);
            const auto &tess = static_cast<const TesselatorMicroBlock &>(*db.GetBlockStaticPart(3)->GetTesselator());

            for (int k1 = 0; k1 < tess.Size(); ++k1)
            {
              for (int j1 = 0; j1 < tess.Size(); ++j1)
              {
                for (int i1 = 0; i1 < tess.Size(); ++i1)
                {
                  data[tess.ToIndex({ i1, j1, k1 })] = solid(sw.x() + i + i1 / float(tess.Size()), sw.y() + j + j1 / float(tess.Size()), sw.z() + k + k1 / float(tess.Size()));
                }
              }
            }
            sector.SetBlockDynamic({ i,j,k }, std::move(dyn));
          }
          else
            sector.SetBlockId({ i,j,k }, 2);
        }
        else
          sector.SetBlockId({ i,j,k }, 0);
      }
    }
  }
}
