#pragma once
#ifndef WorldGeneratorFlat_h__
#define WorldGeneratorFlat_h__

#include "IMapGenerator.h"


class WorldGeneratorFlat : public IMapGenerator
{
public:
  WorldGeneratorFlat(const DataBase &db);
  WorldGeneratorFlat(const DataBase &db, const std::string & id);
  ~WorldGeneratorFlat();

  void Generate(Sector &sector) const override;

private:
  std::string top_layer = "grass";
};



#endif // WorldGeneratorFlat_h__
