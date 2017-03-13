#include "BlockAutoCrafter.h"
#include "accessors\AccessorItem.h"
#include "DataBase.h"
#include "..\imgui\imgui.h"
#include "RecipeBurn.h"
#include "RecipeHand.h"


using factory = TemplateFactory3<std::string, Block, void(const DataBase &, const rapidjson::Value &)>;




BlockAutoCrafter::BlockAutoCrafter(const DataBase & db, const rapidjson::Value &val)
  : Block(db, val),
  mCrafter(CrafterType("Crafter1", db, val), CrafterFast("Crafter1", db, val)),
  mGenerator(CrafterType("Crafter2", db, val), CrafterFast("Crafter2", db, val))
{
  auto t1 = factory::create("", db, val);
  auto t2 = factory::Register<BlockAutoCrafter>::add("");
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

std::unique_ptr<IRecipe> BlockAutoCrafter::CrafterType(const char *type, const DataBase & db, const rapidjson::Value &val) const
{
  if (val.HasMember(type))
  {
    const rapidjson::Value &crafter = val[type];
    if (crafter.HasMember("Recipe"))
    {
      std::string type = crafter["Recipe"].GetString();
      return RecipeFactory::Get().Create(type);
    }
    return{};
  }
  return{};
}

bool BlockAutoCrafter::CrafterFast(const char *type, const DataBase & db, const rapidjson::Value &val) const
{
  if (val.HasMember(type))
  {
    const rapidjson::Value &crafter = val[type];
    if (crafter.HasMember("fast") && crafter["fast"].IsBool())
    {
      return crafter["fast"].GetBool();
    }
  }

  return false;
}

