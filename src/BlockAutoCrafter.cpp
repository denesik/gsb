#include "BlockAutoCrafter.h"
#include "agent\agents\Chest.h"
#include "BlocksDataBase.h"
#include "..\imgui\imgui.h"
#include "RecipeBurn.h"
#include "Recipe.h"



BlockAutoCrafter::BlockAutoCrafter()
  : mCrafter(std::unique_ptr<IRecipe>(new Recipe)),
    mGenerator(std::unique_ptr<IRecipe>(new RecipeBurn))
{
}


BlockAutoCrafter::~BlockAutoCrafter()
{
}

BlockAutoCrafter::BlockAutoCrafter(const BlockAutoCrafter &other)
  : BlockDynamicPart(other), 
    mCrafter(std::unique_ptr<IRecipe>(new Recipe)),
    mGenerator(std::unique_ptr<IRecipe>(new RecipeBurn))
{
  
}

std::unique_ptr<BlockDynamicPart> BlockAutoCrafter::Clone()
{
  BlockDynamicPart *ptr = new BlockAutoCrafter(*this);
  std::unique_ptr<BlockDynamicPart> obj;
  obj.reset(ptr);
  return std::move(obj);
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
    auto &output = *mAgents[3];
    mGenerator.SetInput(input);
    mGenerator.SetOutput(output);

    ImGui::PushID(0);
    input.DrawGui(dt);
    ImGui::PopID();

    ImGui::SameLine();

    ImGui::PushID(1);
    output.DrawGui(dt);
    ImGui::PopID();

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
