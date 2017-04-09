#pragma once
#include <vector>
#include <mapgen_types.h>
#include <tools/Common.h>
#include <boost/icl/interval_map.hpp>

class Sector;
class DataBase;

using BlockInterval = boost::icl::interval<unsigned short>;
using Layering = boost::icl::interval_map<unsigned short, BlockId>;

class GSB_NOVTABLE IMapGenerator
{
public:
  IMapGenerator() { }
  virtual ~IMapGenerator() = default;

  virtual Layering GetLayering(const DataBase &db, int x, int z) const = 0;
  virtual unsigned short GetGroundLevel(const DataBase &db, int x, int z) const = 0;
  virtual const std::string &GetBiome(const DataBase &db, int x, int z) const = 0;
};
