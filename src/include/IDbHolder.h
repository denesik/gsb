#pragma once
#include <tools\Common.h>

class DataBase;

class GSB_NOVTABLE NoDBHolder { };

class GSB_NOVTABLE DBHolder
{
public:
  DBHolder(const DataBase &db)
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