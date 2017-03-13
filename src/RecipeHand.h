#pragma once
#ifndef Recipe_h__
#define Recipe_h__

#include "TemplateFactory.h"
#include "IRecipe.h"
#include "tools/Crc32.h"

class RecipeHand;
namespace
{
  static auto RecipeHandLoaded = IRecipe::factory::Register<RecipeHand>::add("RecipeHand");
}

// TODO_Recipe 
// Этого бесполезного класса также не будет.
class RecipeHand : public INumeredRecipe<gsb::crc32<std::string>()("RecipeHand")>
{
public:
  std::unique_ptr<IRecipe> Clone() override
  {
    return std::unique_ptr<IRecipe>(new RecipeHand(*this));
  }
};

//REGISTER_RECIPE_CLASS(RecipeHand);

#endif // Recipe_h__
