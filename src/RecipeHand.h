#pragma once
#ifndef Recipe_h__
#define Recipe_h__

#include "TemplateFactory.h"
#include "IRecipe.h"
#include "tools/Crc32.h"

class RecipeHand : public INumeredRecipe<gsb::crc32<std::string>()("RecipeHand")>
{
public:

};

REGISTER_RECIPE_CLASS(RecipeHand);

#endif // Recipe_h__
