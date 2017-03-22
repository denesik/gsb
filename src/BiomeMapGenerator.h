#pragma once
#include <IMapGenerator.h>
#include <IGui.h>
#include "../FastNoise/FastNoise.h"
#include <memory>
#include <array>

enum class ClassicalBiome
{
	Tundra,
	Savanna,
	Desert,
	Swampland,
	Taiga,
	Shrubland,
	Forest,
	Plains,
	SeasonalForest,
	Rainforest,
	BiomeCount
};

class BiomeMapGenerator : public IMapGenerator
{
public:
	BiomeMapGenerator(const DataBase & db, int seed = 0);

	// Inherited via IMapGenerator
	void Generate(Sector & sector) const override;

	void AddBiome(std::unique_ptr<IMapGenerator> && generator, ClassicalBiome biom);

protected:
	std::array<std::unique_ptr<IMapGenerator>, static_cast<size_t>(ClassicalBiome::BiomeCount)> mBiomes;
	int seed;
private:
	mutable FastNoise noise;

	// Inherited via IGui
	virtual void DrawGui(const Magnum::Timeline & dt) override;
	ClassicalBiome BiomeSelector(float temperature, float precipitation) const;
};