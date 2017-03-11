#pragma once
#include <tools/Common.h>

namespace Magnum {
  class Timeline;
}

using GuiFunction = std::function<void(const Magnum::Timeline &dt)>;

class GSB_NOVTABLE IGui
{
public:
  virtual void DrawGui(const Magnum::Timeline &dt) = 0;
};
