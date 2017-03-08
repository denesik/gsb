#pragma once
#include <tools/Common.h>

namespace Magnum {
  class Timeline;
}

class GSB_NOVTABLE IGui
{
public:
  virtual void DrawGui(Magnum::Timeline dt) = 0;
};
