#pragma once
#ifndef RecipeBurn_h__
#define RecipeBurn_h__

#include "TemplateFactory.h"
#include "IRecipe.h"
#include "tools/Crc32.h"

// TODO_Recipe 
// Этого бесполезного класса также не будет.
class RecipeBurn : public INumeredRecipe<gsb::crc32<std::string>()("RecipeBurn")>
{
public:
  std::unique_ptr<IRecipe> Clone() override
  {
    return std::unique_ptr<IRecipe>(new RecipeBurn(*this));
  }
};

REGISTER_RECIPE_CLASS(RecipeBurn);

#endif // RecipeBurn_h__
