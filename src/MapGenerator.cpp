#include "MapGenerator.h"
#include "PerlinNoise.h"
#include "Sector.h"
#include "TesselatorMicroBlock.h"
#include "IMapGenerator.h"
#include "../imgui/imgui.h"

PrimitivaMountains::PrimitivaMountains(const DataBase &db, float power) : IMapGenerator(db), mPower(power)
{
  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::DragFloat("TopDownscaler", &mTopDownscaler); });
  AddCustomParameter([&](const Magnum::Timeline &) { ImGui::DragFloat("BotDownscaler", &mBotDownscaler); });
}

#define GEN_OCT 5
float PrimitivaMountains::flatness(float tx, float ty)
{
  return (PerlinNoise2D(tx, ty, 2, 2, GEN_OCT) + 1) / 2.f;
}
float PrimitivaMountains::dens(float tx, float ty, float tz)
{
  auto flat = 1.f / 5.f;// flatness(tx / 1000.f, ty / 1000.f);
  if (ty < -SECTOR_SIZE)
    return 1;

  if (ty < 0)
    return PerlinNoise3D(tx / mBotDownscaler, ty / mBotDownscaler, tz / mBotDownscaler, 1 + 5 * flat, 2, GEN_OCT) + ((-ty*(SECTOR_SIZE / 1000.f)));

  return PerlinNoise3D(tx / mTopDownscaler, ty / mTopDownscaler, tz / mTopDownscaler, 1 + 5 * flat, 2, GEN_OCT) / ((ty + float(SECTOR_SIZE)) / float(SECTOR_SIZE));
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
bool PrimitivaMountains::is_cluster(float tx, float ty, float tz, float type, float prob)
{
  return cluster(tx + type * 3571, ty + type * 3557, tz + type * 3559) + 1 < prob * 2.f;
}
bool PrimitivaMountains::solid(float tx, float ty, float tz)
{
  return dens(tx, ty, tz + 16) > 0.1;
}
#undef GEN_OCT

void PrimitivaMountains::Generate(Sector &sector)
{
  BlockId air = 0;
  BlockId dirt = m_Db.BlockIdFromName("dirt").value_or(0);
  BlockId grass = m_Db.BlockIdFromName("grass").value_or(0);
  BlockId furnance = m_Db.BlockIdFromName("furnance").value_or(0);

  auto sw = cs::StoW(sector.GetPos());
  for (auto k = 0; k < SECTOR_SIZE; ++k)
  {
    for (auto j = 0; j < SECTOR_SIZE; ++j)
    {
      for (auto i = 0; i < SECTOR_SIZE; ++i)
      {
        if (solid(sw.x() + i, sw.y() + j, sw.z() + k))
        {
          if (!solid(sw.x() + i, sw.y() + j + 1, sw.z() + k) && rand()%20 == 1 && false)
          {
            sector.CreateBlock({ i,j,k }, grass);

            auto &data = TesselatorMicroBlock::ToMicroblockData(*sector.GetTesselatorData({ i,j,k }));
            const auto &tess = static_cast<const TesselatorMicroBlock &>(*m_Db.GetBlockStaticPart(grass)->GetTesselator());

            for (auto k1 = 0; k1 < tess.Size(); ++k1)
            {
              for (auto j1 = 0; j1 < tess.Size(); ++j1)
              {
                for (auto i1 = 0; i1 < tess.Size(); ++i1)
                {
                  data[tess.ToIndex({ i1, j1, k1 })] = solid(sw.x() + i + i1 / float(tess.Size()), sw.y() + j + j1 / float(tess.Size()), sw.z() + k + k1 / float(tess.Size()));
                }
              }
            }
          }
          else
            sector.CreateBlock({ i,j,k }, grass);
        }
        else
          sector.CreateBlock({ i,j,k }, air);
      }
    }
  }
}
