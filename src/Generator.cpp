#include "Generator.h"

Generator::Generator(const Generator &other, Accessor &input, Accessor &output)
  : m_recipe_tag(other.m_recipe_tag)
  , mInput(static_cast<AccessorItem &>(input)), mOutput(static_cast<AccessorItem &>(output))
{

}

Generator::Generator(Generator &&other, Accessor &input, Accessor &output)
  : m_recipe_tag(std::move(other.m_recipe_tag))
  , mInput(static_cast<AccessorItem &>(input)), mOutput(static_cast<AccessorItem &>(output))
{

}

Generator::Generator(IRecipe::Tag tag, bool fast, Accessor &input, Accessor &output)
  : m_recipe_tag(tag)
  , mInput(static_cast<AccessorItem &>(input)), mOutput(static_cast<AccessorItem &>(output))
{

}

void Generator::Update(const Magnum::Timeline & dt, const DataBase & db)
{
}

float Generator::Progress() const
{
  return 0.0f;
}

void Generator::Run()
{
}

void Generator::Stop()
{
}

bool Generator::IsRunned() const
{
  return false;
}

bool Generator::IsReady() const
{
  return false;
}
