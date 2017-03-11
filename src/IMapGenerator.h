#pragma once
#include <tools/Common.h>

class Sector;
class BlocksDataBase;

class GSB_NOVTABLE IMapGenerator
{
public:
  IMapGenerator(const BlocksDataBase &db) : m_Db(db) { }
  virtual ~IMapGenerator() = default;
  virtual void Generate(Sector &sector) = 0;
  
  void UpdateGui(const Magnum::Timeline &);
  template<typename T>
  void AddParameter(const T & parameter, const std::string & name, const std::string & description = "");
  void AddCustomParameter(std::function<void(Magnum::Timeline)> gui);
protected:
  const BlocksDataBase &m_Db;
};
