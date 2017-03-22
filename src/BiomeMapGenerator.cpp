#include "BiomeMapGenerator.h"
#include <Sector.h>
#include <../imgui/imgui.h>
#include "ConfiguratableMapGenerator.h"

BiomeMapGenerator::BiomeMapGenerator(const DataBase &db, int seed) : IMapGenerator(db)
{
	noise.SetSeed(seed);
	noise.SetNoiseType(FastNoise::Perlin);
	noise.SetFrequency(0.5f);

	mBiomes[static_cast<size_t>(ClassicalBiome::Tundra)]         = std::make_unique<ConfiguratableMapGenerator>(db, MapGenConfiguration{ "dirt", "snow" });
	mBiomes[static_cast<size_t>(ClassicalBiome::Savanna)]        = std::make_unique<ConfiguratableMapGenerator>(db, MapGenConfiguration{ "dirt", "grass" });
	mBiomes[static_cast<size_t>(ClassicalBiome::Desert)]         = std::make_unique<ConfiguratableMapGenerator>(db, MapGenConfiguration{ "sand", "sand" });
	mBiomes[static_cast<size_t>(ClassicalBiome::Swampland)]      = std::make_unique<ConfiguratableMapGenerator>(db, MapGenConfiguration{ "dirt", "grass" });
	mBiomes[static_cast<size_t>(ClassicalBiome::Taiga)]          = std::make_unique<ConfiguratableMapGenerator>(db, MapGenConfiguration{ "dirt", "taiga_grass" });
	mBiomes[static_cast<size_t>(ClassicalBiome::Shrubland)]      = std::make_unique<ConfiguratableMapGenerator>(db, MapGenConfiguration{ "dirt", "dirt" });
	mBiomes[static_cast<size_t>(ClassicalBiome::Forest)]         = std::make_unique<ConfiguratableMapGenerator>(db, MapGenConfiguration{ "dirt", "grass" });
	mBiomes[static_cast<size_t>(ClassicalBiome::Plains)]         = std::make_unique<ConfiguratableMapGenerator>(db, MapGenConfiguration{ "dirt", "grass" });
	mBiomes[static_cast<size_t>(ClassicalBiome::SeasonalForest)] = std::make_unique<ConfiguratableMapGenerator>(db, MapGenConfiguration{ "dirt", "seasonforest_grass" });
	mBiomes[static_cast<size_t>(ClassicalBiome::Rainforest)]     = std::make_unique<ConfiguratableMapGenerator>(db, MapGenConfiguration{ "dirt", "rainforest_grass" });
}

void BiomeMapGenerator::Generate(Sector & sector) const
{
  float value = noise.GetNoise(sector.GetPos().x() * 100.f, sector.GetPos().z() * 100.f) / 2.f + 1.f;
  float value2 = noise.GetNoise(sector.GetPos().x() * 123.f, sector.GetPos().z() * 123.f) / 2.f + 1.f;
  
  mBiomes[static_cast<size_t>(BiomeSelector(value, value2))]->Generate(sector);
}

void BiomeMapGenerator::AddBiome(std::unique_ptr<IMapGenerator> && generator, ClassicalBiome biom)
{
	mBiomes[static_cast<size_t>(biom)] = std::move(generator);
}

void BiomeMapGenerator::DrawGui(const Magnum::Timeline & dt)
{
	int i = 0;
	for (auto & biome : mBiomes)
	{
		ImGui::BeginGroup();
		ImGui::BeginChild(ImGui::GetID(reinterpret_cast<void *>(i)), ImVec2{400, 0}, true);
		biome->DrawGui(dt);
		ImGui::EndChild();
		ImGui::EndGroup();
		i++;
	}
}

ClassicalBiome BiomeMapGenerator::BiomeSelector(float temperature, float precipitation) const
{
	precipitation = precipitation * temperature;

	if (temperature < 0.1)
		return ClassicalBiome::Tundra;

	if (precipitation < 0.2) {
		if (temperature < 0.5)
			return ClassicalBiome::Tundra;
		if (temperature < 0.95)
			return ClassicalBiome::Savanna;
		return ClassicalBiome::Desert;
	}

	if ((precipitation > 0.5) && (temperature < 0.7))
		return ClassicalBiome::Swampland;

	if (temperature < 0.5)
		return ClassicalBiome::Taiga;

	if (temperature < 0.97) {
		if (precipitation < 0.35)
			return ClassicalBiome::Shrubland;
		return ClassicalBiome::Forest;
	}

	if (precipitation < 0.45)
		return ClassicalBiome::Plains;

	if (precipitation < 0.9)
		return ClassicalBiome::SeasonalForest;

	return ClassicalBiome::Rainforest;
}
