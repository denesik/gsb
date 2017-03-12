#pragma once
#ifndef RecipeBurn_h__
#define RecipeBurn_h__

#include "TemplateFactory.h"
#include "IRecipe.h"
#include "tools/Crc32.h"


class RecipeBurn : public INumeredRecipe<gsb::crc32<std::string>()("RecipeBurn")>
{
public:

};

REGISTER_RECIPE_CLASS(RecipeBurn);

#endif // RecipeBurn_h__