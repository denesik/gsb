#pragma one
#include <IMapGenerator.h>
#include <IGui.h>
#include "../FastNoise/FastNoise.h"
#include <memory>

class BiomeMapGenerator : public IMapGenerator, public IGui
{
public:
	BiomeMapGenerator(const DataBase & db, int seed = 0);

	// Inherited via IMapGenerator
	void Generate(Sector & sector) override;

	void AddBiome(std::unique_ptr<IMapGenerator> && generator, float size = 1.f, const std::string & name = "");

protected:
	std::vector<std::tuple<std::unique_ptr<IMapGenerator>, float, std::string>> mBiomes;
	int seed;
private:
	FastNoise noise;

	// Inherited via IGui
	virtual void DrawGui(const Magnum::Timeline & dt) override;
};