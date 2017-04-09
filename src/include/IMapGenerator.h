#pragma once
#include <vector>
#include <tools\RangeSet.h>
#include <mapgen_types.h>

class Sector;
class DataBase;

struct Layering
{
  gsb::interval_set<unsigned int, BlockId> layers;
};

class GSB_NOVTABLE IMapGenerator
{
public:
  IMapGenerator(const DataBase &db) : m_Db(db) { }
  virtual ~IMapGenerator() = default;
  virtual void Generate(Sector &sector) const = 0;

  virtual Layering GetLayering(int x, int z) = 0;
  virtual unsigned short GetGroundLevel(int x, int z) = 0;
  virtual const std::string &GetBiome(int x, int z) = 0;

protected:
  const DataBase &m_Db;
private:
  std::vector<GuiFunction> parameters;
};
