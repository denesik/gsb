#pragma once
#ifndef Recipe_h__
#define Recipe_h__

#include "TemplateFactory.h"
#include "IRecipe.h"
#include "tools/Crc32.h"

class Recipe : public INumeredRecipe<gsb::crc32<std::string>()("Recipe")>
{
public:

};

REGISTER_RECIPE_CLASS(Recipe);

#endif // Recipe_h__
