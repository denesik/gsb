#include "BlockAutoCrafter.h"
#include "accessors\AccessorItem.h"
#include "DataBase.h"
#include "..\imgui\imgui.h"

BlockAutoCrafter::BlockAutoCrafter(const BlockAutoCrafter &other)
  : BlockAutoCrafter(other, other.m_sector)
{

}

BlockAutoCrafter::BlockAutoCrafter(BlockAutoCrafter &&other)
  : BlockAutoCrafter(std::move(other), other.m_sector)
{

}

BlockAutoCrafter::BlockAutoCrafter(const BlockAutoCrafter &other, Sector &parent)
  : Block(other, parent),
  mCrafterInput(*GetAccessorByName(other.mCrafterInput.Name())), mCrafterOutput(*GetAccessorByName(other.mCrafterOutput.Name())),
  mGeneratorInput(*GetAccessorByName(other.mGeneratorInput.Name())), mGeneratorOutput(*GetAccessorByName(other.mGeneratorOutput.Name())),
  mCrafter(other.mCrafter, mCrafterInput, mCrafterOutput),
  mGenerator(other.mGenerator, mGeneratorInput, mGeneratorOutput)
{

}

BlockAutoCrafter::BlockAutoCrafter(BlockAutoCrafter &&other, Sector &parent)
  : Block(std::move(other), parent),
  mCrafterInput(*GetAccessorByName(other.mCrafterInput.Name())), mCrafterOutput(*GetAccessorByName(other.mCrafterOutput.Name())),
  mGeneratorInput(*GetAccessorByName(other.mGeneratorInput.Name())), mGeneratorOutput(*GetAccessorByName(other.mGeneratorOutput.Name())),
  mCrafter(std::move(other.mCrafter), mCrafterInput, mCrafterOutput),
  mGenerator(std::move(other.mGenerator), mGeneratorInput, mGeneratorOutput)
{

}

BlockAutoCrafter::BlockAutoCrafter(const DataBase &db, const rapidjson::Value &val, Sector &parent, BlockId id)
  : Block(db, val, parent, id),
  mCrafterInput(*GetAccessor("Crafter1", "Input", val)), mCrafterOutput(*GetAccessor("Crafter1", "Output", val)),
  mGeneratorInput(*GetAccessor("Crafter2", "Input", val)), mGeneratorOutput(*GetAccessor("Crafter2", "Output", val)),
  mCrafter(CrafterType("Crafter1", db, val), CrafterFast("Crafter1", db, val), mCrafterInput, mCrafterOutput),
  mGenerator(CrafterType("Crafter2", db, val), CrafterFast("Crafter2", db, val), mGeneratorInput, mGeneratorOutput)
{

}


std::unique_ptr<Block> BlockAutoCrafter::Clone(Sector &parent)
{
  return std::make_unique<BlockAutoCrafter>(*this, parent);
}

void BlockAutoCrafter::DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx, IContext & context)
{
  ImGui::PushID(0);
  {
    ImGui::PushID(0);
    mCrafterInput.DrawGui(dt, ctx, context);
    ImGui::PopID();

    ImGui::SameLine();

    ImGui::PushID(1);
    mCrafterOutput.DrawGui(dt, ctx, context);
    ImGui::PopID();

    ImGui::ProgressBar(mCrafter.Progress(), ImVec2(0.0f, 0.0f), " ");
  }
  ImGui::PopID();
  ImGui::PushID(1);
  {
    ImGui::PushID(0);
    mGeneratorInput.DrawGui(dt, ctx, context);
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
  if (mGenerator.IsReady())
  {
    if (mCrafter.IsReady())
    {
      if (!mGenerator.IsRunned())
        mGenerator.Run();

      if (mGenerator.IsRunned() && !mCrafter.IsRunned())
        mCrafter.Run();
    }
  }
  else
  {
    if (mCrafter.IsRunned())
    {
      mCrafter.Stop();
    }
  }

  mCrafter.Update(dt, mDb);
  mGenerator.Update(dt, mDb);
}

void BlockAutoCrafter::Draw(const Magnum::Matrix4 &vp, Magnum::AbstractShaderProgram& shader)
{

}

// TODO: �������� ������������� �������.
IRecipe::Tag BlockAutoCrafter::CrafterType(const char *type, const DataBase & db, const rapidjson::Value &val) const
{
  if (val.HasMember(type))
  {
    const rapidjson::Value &crafter = val[type];
    if (crafter.HasMember("Recipe"))
    {
      std::string type = crafter["Recipe"].GetString();
      return *db.RecipeTagFromName(type);
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

boost::optional<Accessor &> BlockAutoCrafter::GetAccessor(const char *type, const char *dir, const rapidjson::Value &val) const
{
  if (val.HasMember(type))
  {
    const rapidjson::Value &crafter = val[type];
    if (crafter.HasMember(dir))
    {
      std::string type = crafter[dir].GetString();
      return GetAccessorByName(type);
    }
    return{};
  }
  return{};
}

