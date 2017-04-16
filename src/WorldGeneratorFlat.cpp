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
  if ((x > 0 && x < 10) || (z > 0 && z < 10))
    return{};

  BlockId dirt = db.BlockIdFromName("dirt").value_or(0);
  BlockId dirt2 = db.BlockIdFromName("dirt2").value_or(0);
  BlockId dirt3 = db.BlockIdFromName("dirt3").value_or(0);
  BlockId dirt4 = db.BlockIdFromName("dirt4").value_or(0);
  BlockId grass = db.BlockIdFromName("grass").value_or(0);
  BlockId water = db.BlockIdFromName("water").value_or(0);
  BlockId furnance = db.BlockIdFromName("furnance").value_or(0);
  BlockId stone = db.BlockIdFromName("stone").value_or(0);

  unsigned short hill_level = GetGroundLevel(db,x,z);
  Layering layering;

  layering.insert(layering.begin(), { BlockInterval::right_open(0, land_level), stone });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level - 15), dirt4 });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level - 9), dirt3 });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level - 4), dirt2 });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level), dirt });
  layering.insert(layering.begin(), { BlockInterval::right_open(hill_level, hill_level + 1), grass });

  if(hill_level + 1 < water_level)
    layering.insert(layering.begin(), { BlockInterval::right_open(hill_level + 1, water_level), water });

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
  if ((x > 0 && x < 10) || (z > 0 && z < 10))
    return{};

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

  layering.insert(layering.begin(), { BlockInterval::right_open(0, land_level), stone });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level - 15), dirt4 });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level - 9), dirt3 });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level - 4), dirt2 });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level), dirt });
  layering.insert(layering.begin(), { BlockInterval::right_open(hill_level, hill_level + 1), grass });

  if (hill_level + 1 < water_level)
    layering.insert(layering.begin(), { BlockInterval::right_open(hill_level + 1, water_level), water });

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
  if ((x > 0 && x < 10) || (z > 0 && z < 10))
    return{};

  BlockId sand = db.BlockIdFromName("sand").value_or(0);
  BlockId sand2 = db.BlockIdFromName("sand2").value_or(0);
  BlockId sand3 = db.BlockIdFromName("sand3").value_or(0);
  BlockId stone = db.BlockIdFromName("stone").value_or(0);

  unsigned short hill_level = GetGroundLevel(db, x, z);
  Layering layering;

  layering.insert(layering.begin(), { BlockInterval::right_open(0, land_level), stone });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level - 15), sand3 });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, hill_level - 9), sand2 });
  layering.insert(layering.begin(), { BlockInterval::right_open(land_level, 999), sand });

  return layering;
}

unsigned short WorldGeneratorDesert::GetGroundLevel(const DataBase & db, int x, int z) const
{
  auto value = static_cast<float>(noise.GetNoise(static_cast<float>(-x), static_cast<float>(-z))) / 2.f + 0.5f;

  return static_cast<unsigned short>(value * hill_multiplier + land_level) + 1;
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
  noise_dist.SetSeed(seed);
  noise_dist.SetNoiseType(FastNoise::Cellular);
  noise_dist.SetCellularReturnType(FastNoise::Distance);
  noise_dist.SetFrequency(0.002f);
  noise_dist.SetCellularDistanceFunction(FastNoise::Euclidean);

  noise.SetSeed(seed);
  noise.SetNoiseType(FastNoise::Cellular);
  noise.SetCellularReturnType(FastNoise::CellValue);
  noise.SetFrequency(0.002f);
  noise.SetCellularDistanceFunction(FastNoise::Euclidean);

  biomes.push_back(&hills);
  biomes.push_back(&ex_hills);
  biomes.push_back(&desert);
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
  return static_cast<int>((noise.GetNoise(static_cast<float>(x), static_cast<float>(z)) / 2.f + 0.5f) * biomes.size());
}

void WorldGeneratorBiome::DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx)
{
  static int ret_type = 0;
  ImGui::SliderInt("return_type", &ret_type, 0, 7);
  noise_dist.SetCellularReturnType(static_cast<FastNoise::CellularReturnType>(ret_type));

  ImGui::BeginGroup();
  ex_hills.DrawGui(dt, ctx);
  ImGui::EndGroup();

  ImGui::BeginGroup();
  hills.DrawGui(dt, ctx);
  ImGui::EndGroup();

  ImGui::BeginGroup();
  desert.DrawGui(dt, ctx);
  ImGui::EndGroup();
}

constexpr float pi() { return 3.1415f; }

constexpr int k_size = 11;

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

float apply_kernel(std::function<int(int, int)> func, int x, int y, decltype(gauss_kernel) kernel)
{
  float sum = 0;
  for (int i = -(k_size / 2); i < k_size / 2; ++i)
    for (int j = -(k_size / 2); j < k_size / 2; ++j)
    {
      sum += func(x + i, y + j) * kernel[i + k_size / 2][j + k_size / 2];
    }

  return sum;
}

unsigned short WorldGeneratorBiome::GetGroundLevel(const DataBase & db, int x, int z) const
{
  //auto biome = CalcBiome(x, z);
  //auto other_biome = biomes[biome]->GetGroundLevel(db, x, z);

  std::map<int, float> biome_values;

  for (size_t i = 0; i < biomes.size(); ++i)
  {
    float biome_value = apply_kernel([&](int x, int y) -> int {return !!(CalcBiome(x, z) == i); }, x, z, gauss_kernel);
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
