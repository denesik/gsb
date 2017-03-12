#include "BlockAutoCrafter.h"
#include "accessors\AccessorItem.h"
#include "DataBase.h"
#include "..\imgui\imgui.h"
#include "RecipeBurn.h"
#include "RecipeHand.h"


BlockAutoCrafter::BlockAutoCrafter(const DataBase & db, const rapidjson::Value &val)
  : Block(db, val),
  mCrafter(db, *CrafterValue("Crafter1", db, val)),
  mGenerator(db, *CrafterValue("Crafter2", db, val))
{

}

BlockAutoCrafter::~BlockAutoCrafter()
{
}

BlockAutoCrafter::BlockAutoCrafter(const BlockAutoCrafter &other)
  : Block(other), 
    mCrafter(other.mCrafter),
    mGenerator(other.mGenerator)
{
  
}

std::unique_ptr<Block> BlockAutoCrafter::Clone()
{
  return std::unique_ptr<Block>(new BlockAutoCrafter(*this));
}

void BlockAutoCrafter::DrawGui(const Magnum::Timeline &dt)
{
  ImGui::PushID(0);
  {
    auto &input = *mAgents[0];
    auto &output = *mAgents[1];
    mCrafter.SetInput(input);
    mCrafter.SetOutput(output);

    ImGui::PushID(0);
    input.DrawGui(dt);
    ImGui::PopID();

    ImGui::SameLine();

    ImGui::PushID(1);
    output.DrawGui(dt);
    ImGui::PopID();

    ImGui::ProgressBar(mCrafter.Progress(), ImVec2(0.0f, 0.0f), " ");
  }
  ImGui::PopID();
  ImGui::PushID(1);
  {
    auto &input = *mAgents[2];
    //auto &output = *mAgents[3];
    mGenerator.SetInput(input);
    mGenerator.SetOutput(input);

    ImGui::PushID(0);
    input.DrawGui(dt);
    ImGui::PopID();

//     ImGui::SameLine();
// 
//     ImGui::PushID(1);
//     output.DrawGui(dt);
//     ImGui::PopID();

    ImGui::ProgressBar(mGenerator.Progress(), ImVec2(0.0f, 0.0f), " ");
  }
  ImGui::PopID();

  Update(dt);
}

void BlockAutoCrafter::Update(const Magnum::Timeline &dt)
{
  if (mGenerator.Ready())
  {
    if (mCrafter.Ready())
    {
      if (!mGenerator.Runned())
        mGenerator.Run();

      if (mGenerator.Runned() && !mCrafter.Runned())
        mCrafter.Run();
    }
  }
  else
  {
    if (mCrafter.Runned())
    {
      mCrafter.Stop();
    }
  }

  mCrafter.Update(dt, *mDb);
  mGenerator.Update(dt, *mDb);
}

boost::optional<const rapidjson::Value&> BlockAutoCrafter::CrafterValue(const char *type, const DataBase & db, const rapidjson::Value &val) const
{
  if (val.HasMember(type))
  {
    const rapidjson::Value &crafter = val[type];
    return {crafter};
  }

  return{};
}

