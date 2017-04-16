#pragma once
#include <IMapGenerator.h>
#include "../FastNoise/FastNoise.h"

class WorldGeneratorFlat : public IMapGenerator
{
public:
  // Inherited via IMapGenerator
  Layering GetLayering(const DataBase & db, int x, int z) const override;
  unsigned short GetGroundLevel(const DataBase & db, int x, int z) const override;
  const std::string & GetBiome(const DataBase & db, int x, int z) const override;
};

class WorldGeneratorHills : public IMapGenerator
{
public:
  // Inherited via IMapGenerator
  Layering GetLayering(const DataBase & db, int x, int z) const override;
  unsigned short GetGroundLevel(const DataBase & db, int x, int z) const override;
  const std::string & GetBiome(const DataBase & db, int x, int z) const override;

  WorldGeneratorHills(int seed = 1234);

private:
  unsigned short water_level = 80;
  unsigned short land_level = 47;
  float hill_multiplier = 33.f;
  float freq = 0.01f;

  mutable FastNoise noise;
};

class WorldGeneratorExtremeHills : public IMapGenerator
{
public:
  // Inherited via IMapGenerator
  Layering GetLayering(const DataBase & db, int x, int z) const override;
  unsigned short GetGroundLevel(const DataBase & db, int x, int z) const override;
  const std::string & GetBiome(const DataBase & db, int x, int z) const override;

  // Унаследовано через IMapGenerator
  virtual void DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx) override;

  WorldGeneratorExtremeHills(int seed = 1234);

private:
  unsigned short water_level = 80;
  unsigned short land_level = 47;
  float hill_multiplier = 66.f;
  float freq = 0.01f;

  mutable FastNoise noise;
};

class WorldGeneratorDesert : public IMapGenerator
{
public:
  // Inherited via IMapGenerator
  Layering GetLayering(const DataBase & db, int x, int z) const override;
  unsigned short GetGroundLevel(const DataBase & db, int x, int z) const override;
  const std::string & GetBiome(const DataBase & db, int x, int z) const override;

  // Унаследовано через IMapGenerator
  virtual void DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx) override;

  WorldGeneratorDesert(int seed = 1234);

private:
  unsigned short water_level = 80;
  unsigned short land_level = 47;
  float hill_multiplier = 11.f;
  float freq = 0.01f;

  mutable FastNoise noise;
};

class WorldGeneratorBiome : public IMapGenerator
{
public:
  WorldGeneratorBiome(int seed = 1234);

  // Inherited via IMapGenerator
  Layering GetLayering(const DataBase & db, int x, int z) const override;
  unsigned short GetGroundLevel(const DataBase & db, int x, int z) const override;
  const std::string & GetBiome(const DataBase & db, int x, int z) const override;

  // Унаследовано через IMapGenerator
  virtual void DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx) override;

private:
  int CalcBiome(int x, int z) const;

  mutable FastNoise noise, noise_dist;

  WorldGeneratorHills hills;
  WorldGeneratorExtremeHills ex_hills;
  WorldGeneratorDesert desert;

  std::vector<IMapGenerator*> biomes;

  WorldGeneratorFlat flat;
};