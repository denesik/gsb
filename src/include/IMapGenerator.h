#pragma once
#include <vector>
#include <mapgen_types.h>
#include <tools/Common.h>
#include <boost/icl/interval_map.hpp>
#include <IGui.h>
#include <list>
#include <tools\CoordSystem.h>

class Sector;
class DataBase;

using BlockInterval = boost::icl::interval<unsigned short>;
using Layering = boost::icl::interval_map<unsigned short, BlockId>;

enum class StructureSize
{
  s32x32,
  s64x64,
  s128x128,
};

struct MapTemplate
{
  std::vector<BlockId> data;
  SPos size;
  SBPos position;
};

class GSB_NOVTABLE IMapGenerator : public IGui
{
public:
  IMapGenerator() { }
  virtual ~IMapGenerator() = default;

  virtual Layering GetLayering(const DataBase &db, int x, int z) const = 0;
  virtual unsigned short GetGroundLevel(const DataBase &db, int x, int z) const = 0;
  virtual const std::string &GetBiome(const DataBase &db, int x, int z) const = 0;

  virtual unsigned short GetWaterLevel(const DataBase &db, int x, int z) const { return 0; }

  // ������ ����� �������, ������������ ��� �������
  virtual std::list<MapTemplate> GetProps(const DataBase &db, SPos pos) const { return{}; }
  virtual std::list<MapTemplate> GetStructures(const DataBase &db, int x, int z, StructureSize size) const { return{}; }

  // ������������ ����� IGui
  virtual void DrawGui(const Magnum::Timeline & dt, GuiCtx & ctx) override; //temp
};
