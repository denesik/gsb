#include "BlockResourceGenerator.h"
#include "accessors\AccessorItem.h"
#include "DataBase.h"
#include "..\imgui\imgui.h"

BlockResourceGenerator::BlockResourceGenerator(const BlockResourceGenerator &other)
  : BlockResourceGenerator(other, other.m_sector)
{

}

BlockResourceGenerator::BlockResourceGenerator(BlockResourceGenerator &&other)
  : BlockResourceGenerator(std::move(other), other.m_sector)
{

}

BlockResourceGenerator::BlockResourceGenerator(const BlockResourceGenerator &other, Sector &parent)
  : Block(other, parent),
  mGeneratorInput(*GetAccessorByName(other.mGeneratorInput.Name())), 
  mGeneratorOutput(*GetAccessorByName(other.mGeneratorOutput.Name())),
  mResourceOutput(*GetAccessorByName(other.mResourceOutput.Name())),
  mGenerator(other.mGenerator, mGeneratorInput, mGeneratorOutput)
{

}

BlockResourceGenerator::BlockResourceGenerator(BlockResourceGenerator &&other, Sector &parent)
  : Block(std::move(other), parent),
  mGeneratorInput(*GetAccessorByName(other.mGeneratorInput.Name())), 
  mGeneratorOutput(*GetAccessorByName(other.mGeneratorOutput.Name())),
  mResourceOutput(*GetAccessorByName(other.mResourceOutput.Name())),
  mGenerator(std::move(other.mGenerator), mGeneratorInput, mGeneratorOutput)
{

}

BlockResourceGenerator::BlockResourceGenerator(const DataBase &db, const rapidjson::Value &val, Sector &parent, BlockId id)
  : Block(db, val, parent, id),
  mGeneratorInput(*GetAccessor("Crafter1", "Input", val)), 
  mGeneratorOutput(*GetAccessor("Crafter1", "Output", val)),
  mResourceOutput(*GetAccessor("Generator1", "Output", val)),
  mGenerator(CrafterType("Crafter1", db, val), CrafterFast("Crafter1", db, val), mGeneratorInput, mGeneratorOutput)
{

}


std::unique_ptr<Block> BlockResourceGenerator::Clone(Sector &parent)
{
  return std::make_unique<BlockResourceGenerator>(*this, parent);
}

void BlockResourceGenerator::DrawGui(const Magnum::Timeline &dt, GuiCtx & ctx, IContext & context)
{
  ImGui::PushID(1);
  {
    ImGui::PushID(0);
    mResourceOutput.DrawGui(dt, ctx, context);
    mGeneratorInput.DrawGui(dt, ctx, context);
    mGeneratorOutput.DrawGui(dt, ctx, context);
    ImGui::PopID();

    ImGui::ProgressBar(mGenerator.Progress(), ImVec2(0.0f, 0.0f), " ");
  }
  ImGui::PopID();

  Update(dt);
}

void BlockResourceGenerator::Update(const Magnum::Timeline &dt)
{
  //if (mGenerator.IsReady())
  //{
  //  if (mCrafter.IsReady())
  //  {
  //    if (!mGenerator.IsRunned())
  //      mGenerator.Run();

  //    if (mGenerator.IsRunned() && !mCrafter.IsRunned())
  //      mCrafter.Run();
  //  }
  //}
  //else
  //{
  //  if (mCrafter.IsRunned())
  //  {
  //    mCrafter.Stop();
  //  }
  //}

  //mCrafter.Update(dt, mDb);
  //mGenerator.Update(dt, mDb);
}

void BlockResourceGenerator::Draw(const Magnum::Matrix4 &vp, Magnum::AbstractShaderProgram& shader)
{

}

// TODO: проверка существования рецепта.
IRecipe::Tag BlockResourceGenerator::CrafterType(const char *type, const DataBase & db, const rapidjson::Value &val) const
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

bool BlockResourceGenerator::CrafterFast(const char *type, const DataBase & db, const rapidjson::Value &val) const
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

boost::optional<Accessor &> BlockResourceGenerator::GetAccessor(const char *type, const char *dir, const rapidjson::Value &val) const
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

