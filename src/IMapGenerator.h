#pragma once
#include <array>
#include "tools/CoordSystem.h"
#include "BlockStaticPart.h"

class Sector;
class BlocksDataBase;

class IMapGenerator
{
public:
  IMapGenerator(const BlocksDataBase &db) : m_Db(db) { }
  virtual ~IMapGenerator() = default;
  virtual void Generate(Sector &sector, std::array<BlockId, SECTOR_CAPACITY> &blocks) = 0;

protected:
  const BlocksDataBase &m_Db;
};
