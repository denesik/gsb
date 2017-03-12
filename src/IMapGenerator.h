#pragma once
#include <tools/Common.h>
#include "../imgui/imgui.h"
#include <functional>
#include <IGui.h>
#include <Magnum\Timeline.h>
#include <tuple>
#include <vector>

class Sector;
class DataBase;

class GSB_NOVTABLE IMapGenerator : public IGui
{
public:
  IMapGenerator(const DataBase &db) : m_Db(db) { }
  virtual ~IMapGenerator() = default;
  virtual void Generate(Sector &sector) = 0;
  
  void DrawGui(const Magnum::Timeline &);
  void AddCustomParameter(GuiFunction gui_update);
protected:
  const BlocksDataBase &m_Db;
private:
  std::vector<GuiFunction> parameters;
};
