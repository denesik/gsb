#include "IMapGenerator.h"

void IMapGenerator::DrawGui(const Magnum::Timeline &tl, GuiCtx & ctx)
{
  for(const auto f : parameters)
	f(tl);
}

void IMapGenerator::AddCustomParameter(GuiFunction gui)
{
  parameters.push_back(gui);
}
