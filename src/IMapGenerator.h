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
  virtual void Generate(Sector &sector) = 0;

protected:
  const BlocksDataBase &m_Db;
};
