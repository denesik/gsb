#pragma once
#include "IMapGenerator.h"
#include "ThreadProcess.h"

class Sector;
class DataBase;

class GSB_NOVTABLE IMapLoader : public ThreadProcess<IMapLoader>
{
public:
  IMapLoader(const IMapGenerator &generator, const DataBase &db);
  ~IMapLoader();

  void SetSector(std::weak_ptr<Sector> sector);

  virtual void Process() = 0;

protected:
  std::weak_ptr<Sector> mSector;
  const IMapGenerator &mGenerator;
  const DataBase &mDb;
};