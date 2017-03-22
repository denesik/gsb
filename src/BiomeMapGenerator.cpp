#include "BiomeMapGenerator.h"
#include <Sector.h>
#include <../imgui/imgui.h>

BiomeMapGenerator::BiomeMapGenerator(const DataBase &db, int seed) : IMapGenerator(db)
{
	noise.SetSeed(seed);
	noise.SetNoiseType(FastNoise::Cellular);
}

void BiomeMapGenerator::Generate(Sector & sector) const
{
	float value = noise.GetNoise(sector.GetPos().x() * 100.f, sector.GetPos().z() * 100.f);
  float normalized = value / 2.f + 1.f;
  int biome = static_cast<int>(normalized * (mBiomes.size() - 1));

	std::get<0>(mBiomes[biome])->Generate(sector);
}

void BiomeMapGenerator::AddBiome(std::unique_ptr<IMapGenerator> && generator, float size, const std::string & name)
{
	mBiomes.push_back(std::make_tuple(std::move(generator), size, name));
}

void BiomeMapGenerator::DrawGui(const Magnum::Timeline & dt)
{
	int i = 0;
	for (auto & biome : mBiomes)
	{
		ImGui::BeginGroup();
		ImGui::Text(std::get<2>(biome).c_str());
		ImGui::BeginChild(ImGui::GetID(reinterpret_cast<void *>(i)), ImVec2{400, 0}, true);
		std::get<0>(biome)->DrawGui(dt);
		ImGui::EndChild();
		ImGui::EndGroup();
		i++;
	}
}
