#pragma once
#ifndef WorldGeneratorTest_h__
#define WorldGeneratorTest_h__


#include "IMapGenerator.h"


class WorldGeneratorTest : public IMapGenerator
{
public:
  WorldGeneratorTest(const DataBase &db);
  ~WorldGeneratorTest();

  void Generate(Sector &sector) const override;
};



#endif // WorldGeneratorTest_h__
