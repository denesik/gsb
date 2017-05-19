#pragma once
#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT
#include <IMapGenerator.h>

namespace gsb {

  class BiomeGenerator : public IMapGenerator {
  public:
    
    BiomeGenerator();

    // Inherited via IMapGenerator
    virtual void Generate(Sector & sector) const override;

    virtual gsb::interval_set<unsigned int, BlockId> GetLayering(const DataBase &db, int x, int z) override;

    virtual unsigned short GetGroundLevel(const DataBase &db, int x, int z) override;

    virtual const std::string & GetBiome(const DataBase &db, int x, int z) override;

  };

  // Exporting `my_namespace::plugin` variable with alias name `plugin`
  // (Has the same effect as `BOOST_DLL_ALIAS(my_namespace::plugin, plugin)`)
  extern "C" BOOST_SYMBOL_EXPORT BiomeGenerator biome_generator;
  BiomeGenerator biome_generator;

} // namespace my_namespace
