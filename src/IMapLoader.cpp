#include "IMapLoader.h"

IMapLoader::IMapLoader(std::unique_ptr<IMapGenerator> gen) : mGenerator(std::move(gen))
{
}

IMapGenerator & IMapLoader::GetGenerator()
{
	if (mGenerator)
		return *mGenerator;
}
