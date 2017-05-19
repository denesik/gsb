#pragma once
#include "IMapGenerator.h"
//#include "ThreadProcess.h"

class Sector;
class DataBase;

class /*GSB_NOVTABLE*/ IMapLoader/* GSB_ABSTRACT : public ThreadProcess<IMapLoader>*/
{
public:
  IMapLoader(const DataBase &db);
  virtual ~IMapLoader();

  virtual void Process(Sector &sector) = 0;

protected:
  const DataBase &mDb;
};

// class GSB_NOVTABLE IMapSaver GSB_ABSTRACT : public ThreadProcess<IMapSaver>
// {
// public:
//   IMapSaver(const DataBase &db);
//   virtual ~IMapSaver();
// 
//   void SetSector(Sector * sector);
// 
//   virtual void Process() = 0;
// 
// protected:
//   Sector * mSector;
//   const DataBase &mDb;
// };
