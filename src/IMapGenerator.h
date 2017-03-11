#pragma once
#include <tools/Common.h>
#include "../imgui/imgui.h"
#include <functional>
#include <IGui.h>
//#include <Magnum\Timeline.h>
#include <tuple>

class Sector;
class BlocksDataBase;

class GSB_NOVTABLE IMapGenerator : public IGui
{
public:
  IMapGenerator(const BlocksDataBase &db) : m_Db(db) { }
  virtual ~IMapGenerator() = default;
  virtual void Generate(Sector &sector) = 0;
  
   void DrawGui(const Magnum::Timeline &);
   void AddCustomParameter(GuiFunction gui_update);

   template<typename T> void AddParameter(T & out, const std::string & name, const std::string & description)
   {
	   throw NotImplemented();
   }
   template<> void AddParameter(float & out, const std::string & name, const std::string & description)
   {
	   ImGui::InputFloat(name.c_str(), &out);
   }

   template<typename T> void AddSlider(std::tuple<T, T> min_max, T & out, const std::string & name, const std::string & description = "")
   {
	   ImGui::SliderFloat(name.c_str(), &out, std::get<0>(min_max), std::get<1>(min_max));
   }
protected:
  const BlocksDataBase &m_Db;
};
