#pragma once
#include <Magnum/SceneGraph/AbstractFeature.h>
#include "CoordSystem.h"

namespace Magnum {
  class DebugLines;
}

class Brezenham
{
public:
  static std::tuple<Magnum::Vector3i, Magnum::Vector3> PickFirst(
    const Magnum::Vector3 &origin,
    const Magnum::Vector3 &direction,
    float radius,
    std::function<bool(const WBPos &)> collider,
    Magnum::DebugLines * lines_render = nullptr
  );
};
