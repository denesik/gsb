#pragma once
#ifndef WorldGeneratorFlat_h__
#define WorldGeneratorFlat_h__

#include "IMapGenerator.h"


class WorldGeneratorFlat : public IMapGenerator
{
public:
  WorldGeneratorFlat(const DataBase &db);
  ~WorldGeneratorFlat();

  void Generate(Sector &sector) const override;
};



#endif // WorldGeneratorFlat_h__
