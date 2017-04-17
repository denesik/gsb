#include "WorldGeneratorFlat.h"
#include <../imgui/imgui.h>
#include "DataBase.h"
#include <vector>
#include <limits>
#include <cmath>
#include <map>

#define ADDL layering.insert(layering.begin(), { BlockInterval::right_open(

Layering WorldGeneratorFlat::GetLayering(const DataBase & db, int x, int z) const
{
  const int dirt_level = 64;
  BlockId dirt = db.BlockIdFromName("dirt").value_or(0);
  BlockId grass = db.BlockIdFromName("grass").value_or(0);
  BlockId furnance = db.BlockIdFromName("furnance").value_or(0);
  BlockId number = db.BlockIdFromName("number").value_or(0);
  BlockId world_side = db.BlockIdFromName("world_side").value_or(0);

  Layering layering;
  layering += std::make_pair(boost::icl::interval<unsigned short>::right_open(0, 64), dirt);
  layering += std::make_pair(boost::icl::interval<unsigned short>::right_open(64, 65), grass);
  if ((rand() % 100 == 1 || rand() % 100 == 2))
    layering += std::make_pair(boost::icl::interval<unsigned short>::right_open(65, 66), world_side);

  return layering;
}

unsigned short WorldGeneratorFlat::GetGroundLevel(const DataBase & db, int x, int z) const
{
  return 64;
}

const std::string & WorldGeneratorFlat::GetBiome(const DataBase & db, int x, int z) const
{
  static const std::string b = "flat";
  return b;
}

//-------------------------------------------------------

Layering WorldGeneratorHills::GetLayering(const DataBase & db, int x, int z) const
{
  BlockId dirt = db.BlockIdFromName("dirt").value_or(0);
  BlockId dirt2 = db.BlockIdFromName("dirt2").value_or(0);
  BlockId dirt3 = db.BlockIdFromName("dirt3").value_or(0);
  BlockId dirt4 = db.BlockIdFromName("dirt4").value_or(0);
  BlockId grass = db.BlockIdFromName("grass").value_or(0);
  BlockId water = db.BlockIdFromName("water").value_or(0);
  BlockId furnance = db.BlockIdFromName("furnance").value_or(0);
  BlockId stone = db.BlockIdFromName("stone").value_or(0);

  unsigned short hill_level = GetGroundLevel(db, x, z);
  Layering layering;

  layering.set({ BlockInterval::right_open(0, 1), grass });
  layering.set({ BlockInterval::right_open(1, 5), dirt });
  layering.set({ BlockInterval::right_open(5, 10), dirt2 });
  layering.set({ BlockInterval::right_open(10, 15), dirt3 });
  layering.set({ BlockInterval::right_open(15, 25), dirt4 });
  layering.set({ BlockInterval::right_open(25, 999), stone });

  //if (hill_level + 1 < water_level)
  //  layering.insert(layering.begin(), { BlockInterval::right_open(hill_level + 1, water_level), water });

  return layering;
}

unsigned short WorldGeneratorHills::GetGroundLevel(const DataBase & db, int x, int z) const
{
  auto value = static_cast<float>(noise.GetNoise(static_cast<float>(x), static_cast<float>(z))) / 2.f + 0.5f;

  return static_cast<unsigned short>(value * hill_multiplier + land_level) + 1;
}

const std::string & WorldGeneratorHills::GetBiome(const DataBase & db, int x, int z) const
{
  static const std::string b = "hills";
  return b;
}

WorldGeneratorHills::WorldGeneratorHills(int seed)
{
  noise.SetSeed(seed);
  noise.SetNoiseType(FastNoise::SimplexFractal);
}

//-------------------------------------------------------

Layering WorldGeneratorExtremeHills::GetLayering(const DataBase & db, int x, int z) const
{
  BlockId dirt = db.BlockIdFromName("dirt").value_or(0);
  BlockId dirt2 = db.BlockIdFromName("dirt2").value_or(0);
  BlockId dirt3 = db.BlockIdFromName("dirt3").value_or(0);
  BlockId dirt4 = db.BlockIdFromName("dirt4").value_or(0);
  BlockId grass = db.BlockIdFromName("grass").value_or(0);
  BlockId water = db.BlockIdFromName("water").value_or(0);
  BlockId furnance = db.BlockIdFromName("furnance").value_or(0);
  BlockId stone = db.BlockIdFromName("stone").value_or(0);

  unsigned short hill_level = GetGroundLevel(db, x, z);
  Layering layering;

  layering.set({ BlockInterval::right_open(0, 1), grass });
  layering.set({ BlockInterval::right_open(1, 5), dirt });
  layering.set({ BlockInterval::right_open(5, 10), dirt2 });
  layering.set({ BlockInterval::right_open(10, 15), dirt3 });
  layering.set({ BlockInterval::right_open(15, 25), dirt4 });
  layering.set({ BlockInterval::right_open(25, 999), stone });

  //if (hill_level + 1 < water_level)
  //  layering.insert(layering.begin(), { BlockInterval::right_open(hill_level + 1, water_level), water });

  return layering;
}

unsigned short WorldGeneratorExtremeHills::GetGroundLevel(const DataBase & db, int x, int z) const
{
  auto value = static_cast<float>(noise.GetNoise(static_cast<float>(-x), static_cast<float>(-z))) / 2.f + 0.5f;

  return static_cast<unsigned short>(value * hill_multiplier + land_level) + 1;
}

const std::string & WorldGeneratorExtremeHills::GetBiome(const DataBase & db, int x, int z) const
{
  static const std::string b = "extreme hills";
  return b;
}

WorldGeneratorExtremeHills::WorldGeneratorExtremeHills(int seed)
{
  noise.SetSeed(seed);
  noise.SetNoiseType(FastNoise::SimplexFractal);
}

void WorldGeneratorExtremeHills::DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx)
{
}

//-------------------------------------------------------

Layering WorldGeneratorDesert::GetLayering(const DataBase & db, int x, int z) const
{
  BlockId sand = db.BlockIdFromName("sand").value_or(0);
  BlockId sand2 = db.BlockIdFromName("sand2").value_or(0);
  BlockId sand3 = db.BlockIdFromName("sand3").value_or(0);
  BlockId stone = db.BlockIdFromName("stone").value_or(0);

  unsigned short hill_level = GetGroundLevel(db, x, z);
  Layering layering;

  layering.set({ BlockInterval::right_open(0, 1), sand });
  layering.set({ BlockInterval::right_open(1, 5), sand2 });
  layering.set({ BlockInterval::right_open(5, 10), sand3 });
  layering.set({ BlockInterval::right_open(10, 999), stone });

  return layering;
}

unsigned short WorldGeneratorDesert::GetGroundLevel(const DataBase & db, int x, int z) const
{
  auto value = static_cast<float>(noise.GetNoise(static_cast<float>(-x), static_cast<float>(-z))) / 2.f + 0.5f;

  return static_cast<unsigned short>(value * hill_multiplier + 47) + 1;
}

const std::string & WorldGeneratorDesert::GetBiome(const DataBase & db, int x, int z) const
{
  static const std::string b = "extreme hills";
  return b;
}

WorldGeneratorDesert::WorldGeneratorDesert(int seed)
{
  noise.SetSeed(seed);
  noise.SetNoiseType(FastNoise::SimplexFractal);
}

void WorldGeneratorDesert::DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx)
{
}

//-------------------------------------------------------

WorldGeneratorBiome::WorldGeneratorBiome(int seed)
  : hills(seed)
{
  noise.SetSeed(seed);
  noise.SetNoiseType(FastNoise::Cellular);
  noise.SetCellularReturnType(FastNoise::CellValue);
  noise.SetFrequency(0.02f);
  noise.SetCellularDistanceFunction(FastNoise::Euclidean);

  biomes.push_back(&hills);
  biomes.push_back(&ex_hills);
  biomes.push_back(&desert);
  biomes.push_back(&rock_desert);
  biomes.push_back(&swamp);
}

Layering WorldGeneratorBiome::GetLayering(const DataBase & db, int x, int z) const
{
  auto biome = CalcBiome(x, z);
  return biomes[biome]->GetLayering(db, x, z);
}

float lerp(float t, float a, float b) {
  return (1 - t)*a + t*b;
}

int WorldGeneratorBiome::CalcBiome(int x, int z) const
{
  float fx = static_cast<float>(x), fz = static_cast<float>(z);
  noise.GradientPerturbFractal(fx, fz);
  return static_cast<int>((noise.GetNoise(fx, fz) / 2.f + 0.5f) * biomes.size());
}

void WorldGeneratorBiome::DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx)
{
 /* static int ret_type = 0;
  ImGui::SliderInt("return_type", &ret_type, 0, 7);
  noise_dist.SetCellularReturnType(static_cast<FastNoise::CellularReturnType>(ret_type));*/

  for (size_t i = 0; i < biomes.size(); ++i)
  {
    ImGui::BeginGroup();
    biomes[i]->DrawGui(dt, ctx);
    ImGui::EndGroup();
  }
}

constexpr float pi() { return 3.1415f; }

constexpr int k_size = 21;

auto gauss_kernel = []()
{
  const float sigma = 1;
  std::array<std::array<float, k_size>, k_size> kernel;
  const float mean = k_size / 2.f;
  float sum = 0.f;
  for (int x = 0; x < k_size; ++x)
    for (int y = 0; y < k_size; ++y)
    {
      kernel[x][y] = exp(-0.5f * (pow((x - mean) / sigma, 2.f) + pow((y - mean) / sigma, 2.f)))
        / (2.f * pi() * sigma * sigma);

      sum += kernel[x][y];
    }

  for (int x = 0; x < k_size; ++x)
    for (int y = 0; y < k_size; ++y)
      kernel[x][y] /= sum;

  return kernel;
}();



constexpr std::array<std::array<float, 21>, 21> box_kernel
{
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f },
    std::array<float, 21>{ 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f, 1.f / 441.f }
};

template<int size>
float apply_kernel(std::function<int(int, int)> func, int x, int y, std::array<std::array<float, size>, size> kernel)
{
  float sum = 0;
  for (int i = -(size / 2); i < size / 2; ++i)
    for (int j = -(size / 2); j < size / 2; ++j)
    {
      sum += func(x + i, y + j) * kernel[i + size / 2][j + size / 2];
    }

  return sum;
}

unsigned short WorldGeneratorBiome::GetGroundLevel(const DataBase & db, int x, int z) const
{
  std::map<int, float> biome_values;

  for (size_t i = 0; i < biomes.size(); ++i)
  {
    float biome_value = apply_kernel([&](int x, int y) -> int {return !!(CalcBiome(x, z) == i); }, x, z, box_kernel);
    if (biome_value > 0.01)
      biome_values[i] = biome_value;
  }

  if (biome_values.empty())
    return 0;

  float total_sum = 0;
  for (auto &i = biome_values.begin(); i != biome_values.end(); i++)
  {
    total_sum += std::get<1>(*i) * biomes[std::get<0>(*i)]->GetGroundLevel(db, x, z);
  }

  return static_cast<unsigned short>(total_sum);
}

const std::string & WorldGeneratorBiome::GetBiome(const DataBase & db, int x, int z) const
{
  auto biome = CalcBiome(x, z);
  return biomes[biome]->GetBiome(db, x, z);
}

unsigned short WorldGeneratorBiome::GetWaterLevel(const DataBase & db, int x, int z) const
{
  auto biome = CalcBiome(x, z);
  return biomes[biome]->GetWaterLevel(db, x, z);
}

//-------------------------------------------------------

Layering WorldGeneratorRockDesert::GetLayering(const DataBase & db, int x, int z) const
{
  BlockId stone = db.BlockIdFromName("stone").value_or(0);

  Layering layering;

  layering.set({ BlockInterval::right_open(0, 999), stone });

  return layering;
}

unsigned short WorldGeneratorRockDesert::GetGroundLevel(const DataBase & db, int x, int z) const
{
  auto value = static_cast<float>(noise.GetNoise(static_cast<float>(-x), static_cast<float>(-z))) / 2.f + 0.5f;

  return static_cast<unsigned short>(value * hill_multiplier + 47) + 1;
}

const std::string & WorldGeneratorRockDesert::GetBiome(const DataBase & db, int x, int z) const
{
  static const std::string b = "stone desert";
  return b;
}

WorldGeneratorRockDesert::WorldGeneratorRockDesert(int seed)
{
  noise.SetSeed(1234 - seed);
  noise.SetNoiseType(FastNoise::SimplexFractal);
}

//-------------------------------------------------------

Layering WorldGeneratorSwamp::GetLayering(const DataBase & db, int x, int z) const
{
  BlockId sapropel = db.BlockIdFromName("sapropel").value_or(0);
  BlockId peat = db.BlockIdFromName("peat").value_or(0);
  BlockId peat_grass = db.BlockIdFromName("peat_grass").value_or(0);
  BlockId stone = db.BlockIdFromName("stone").value_or(0);

  auto value = static_cast<float>(noise.GetNoise(static_cast<float>(-x), static_cast<float>(-z))) / 2.f + 0.5f;
  auto depth = static_cast<unsigned short>(value * hill_multiplier * 3) + 1;
  Layering layering;

  layering.set({ BlockInterval::right_open(0, 1), peat_grass });
  layering.set({ BlockInterval::right_open(1, depth), peat });
  layering.set({ BlockInterval::right_open(depth, depth + 2), sapropel });
  layering.set({ BlockInterval::right_open(depth + 2, 999), stone });

  return layering;
}

unsigned short WorldGeneratorSwamp::GetGroundLevel(const DataBase & db, int x, int z) const
{
  auto value = static_cast<float>(noise.GetNoise(static_cast<float>(-x), static_cast<float>(-z))) / 2.f + 0.5f;

  return static_cast<unsigned short>(value * hill_multiplier + 50) + 1;
}

unsigned short WorldGeneratorSwamp::GetWaterLevel(const DataBase & db, int x, int z) const
{
  return static_cast<unsigned short>(w_level);
}

const std::string & WorldGeneratorSwamp::GetBiome(const DataBase & db, int x, int z) const
{
  static const std::string b = "swamp";
  return b;
}

WorldGeneratorSwamp::WorldGeneratorSwamp(int seed)
{
  noise.SetSeed(4356 - seed);
  noise.SetNoiseType(FastNoise::SimplexFractal);
}

void WorldGeneratorSwamp::DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx)
{
  ImGui::SliderInt("water level", &w_level, 0, 255);
}
