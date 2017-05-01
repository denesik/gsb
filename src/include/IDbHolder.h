#pragma once
#include <tools\Common.h>

class DataBase;

class GSB_NOVTABLE IDBHolder
{
public:
  IDBHolder(const DataBase &db)
    : mDB(db)
  {

  }

  const DataBase &GetDataBase()
  {
    return mDB;
  }

private:
  const DataBase &mDB;
};