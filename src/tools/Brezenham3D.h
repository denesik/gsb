#pragma once
#include <Magnum/SceneGraph/AbstractFeature.h>
#include "CoordSystem.h"
#include <vector>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector2.h>

/*
namespace Magnum {
  class DebugLines;
}

class Brezenham
{
public:
  static std::tuple<WBPos, Magnum::Vector3> PickFirst(
    const Magnum::Vector3 &origin,
    const Magnum::Vector3 &direction,
    float radius,
    std::function<bool(const WBPos &)> collider,
    Magnum::DebugLines * lines_render = nullptr
  );
};*/

std::vector<Magnum::Vector3i> voxel_traversal(const Magnum::Vector3 &ray_start, const Magnum::Vector3 &ray_end);