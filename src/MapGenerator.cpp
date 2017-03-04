#include "MapGenerator.h"
#include "PerlinNoise.h"
#include "Sector.h"

PrimitivaMountains::PrimitivaMountains(const BlocksDataBase &db, float power) : IMapGenerator(db), mPower(power)
{
}

#define GEN_OCT 2
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
  return dens(tx, ty, tz) > 0.1;
}
#undef GEN_OCT

void PrimitivaMountains::Generate(Sector &sector, std::array<BlockId, SECTOR_CAPACITY> &blocks)
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
          blocks[cs::SBtoBI({ i,j,k })] = 3;
        }
        else
          blocks[cs::SBtoBI({ i,j,k })] = 0;
      }
    }
  }
}
