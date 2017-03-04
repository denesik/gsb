#include "Brezenham3D.h"
#include <algorithm>
#include <Magnum/Math/Vector3.h>
#include <Magnum/SceneGraph/AbstractFeature.h>
#include "CoordSystem.h"
#include "DebugLines.h"

int mod(int value, int modulus) {
  return (value % modulus + modulus) % modulus;
}

template <typename T> 
T intbound(T s, T ds) {
  if (ds < 0)
    return intbound(-s, -ds);

  s = mod(s, T(1));
  return (1 - s) / ds;
}

template <typename T> 
int signum(T x) {
  return x > T(0) ? T(1) : x < T(0) ? T(-1) : T(0);
}


std::tuple<WBPos, Magnum::Vector3> Brezenham::PickFirst(
  const Magnum::Vector3 &origin, 
  const Magnum::Vector3 &direction, 
  float radius, 
  std::function<bool(const WBPos &)> collider,
  Magnum::DebugLines * lines_render
)
{
  // From "A Fast Voxel Traversal Algorithm for Ray Tracing"
  // by John Amanatides and Andrew Woo, 1987
  // <http://www.cse.yorku.ca/~amana/research/grid.pdf>
  // <http://citeseer.ist.psu.edu/viewdoc/summary?doi=10.1.1.42.3443>
  int x = origin[0];
  int y = origin[1];
  int z = origin[2];
  float dx = direction[0];
  float dy = direction[1];
  float dz = direction[2];
  float stepX = signum(dx);
  float stepY = signum(dy);
  float stepZ = signum(dz);
  float tMaxX = intbound(origin[0], dx);
  float tMaxY = intbound(origin[1], dy);
  float tMaxZ = intbound(origin[2], dz);
  float tDeltaX = stepX / dx;
  float tDeltaY = stepY / dy;
  float tDeltaZ = stepZ / dz;
  auto face = Magnum::Vector3();

  if (dx == 0 && dy == 0 && dz == 0)
    throw std::logic_error("Raycast in zero direction!");

  radius /= sqrt(dx*dx + dy*dy + dz*dz);

  while (true) 
  {
    if (collider({ x, y, z }))
      break;

    if (tMaxX < tMaxY) 
    {
      if (tMaxX < tMaxZ) 
      {
        if (tMaxX > radius) break;
        x += stepX;
        tMaxX += tDeltaX;
        face[0] = -stepX;
        face[1] = 0;
        face[2] = 0;
      }
      else 
      {
        if (tMaxZ > radius) break;
        z += stepZ;
        tMaxZ += tDeltaZ;
        face[0] = 0;
        face[1] = 0;
        face[2] = -stepZ;
      }
    }
    else 
    {
      if (tMaxY < tMaxZ) 
      {
        if (tMaxY > radius) break;
        y += stepY;
        tMaxY += tDeltaY;
        face[0] = 0;
        face[1] = -stepY;
        face[2] = 0;
      }
      else 
      {
        if (tMaxZ > radius) break;
        z += stepZ;
        tMaxZ += tDeltaZ;
        face[0] = 0;
        face[1] = 0;
        face[2] = -stepZ;
      }
    }
  }

  return std::make_tuple(Magnum::Vector3i(x, y, z), face);
}