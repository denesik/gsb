#include "Brezenham3D.h"
#include <algorithm>
#include <Magnum/Math/Vector3.h>
#include <Magnum/SceneGraph/AbstractFeature.h>
#include "CoordSystem.h"
#include "DebugLines.h"
#include <limits>

const float _bin_size = 1;

std::vector<Magnum::Vector3i> voxel_traversal(const Magnum::Vector3 &ray_start, const Magnum::Vector3 & ray_end) {
  std::vector<Magnum::Vector3i> visited_voxels;

  // This id of the first/current voxel hit by the ray.
  // Using floor (round down) is actually very important,
  // the implicit int-casting will round up for negative numbers.
  Magnum::Vector3i current_voxel(static_cast<int>(std::floor(ray_start[0] / _bin_size)),
    static_cast<int>(std::floor(ray_start[1] / _bin_size)),
    static_cast<int>(std::floor(ray_start[2] / _bin_size)));

  // The id of the last voxel hit by the ray.
  // TODO: what happens if the end point is on a border?
  Magnum::Vector3i last_voxel(static_cast<int>(std::floor(ray_end[0] / _bin_size)),
    static_cast<int>(std::floor(ray_end[1] / _bin_size)),
    static_cast<int>(std::floor(ray_end[2] / _bin_size)));

  // Compute normalized ray direction.
  Magnum::Vector3 ray = ray_end - ray_start;
  //ray.normalize();

  // In which direction the voxel ids are incremented.
  float stepX = (ray[0] >= 0) ? 1.f : -1.f; // correct
  float stepY = (ray[1] >= 0) ? 1.f : -1.f; // correct
  float stepZ = (ray[2] >= 0) ? 1.f : -1.f; // correct

                                         // Distance along the ray to the next voxel border from the current position (tMaxX, tMaxY, tMaxZ).
  float next_voxel_boundary_x = (current_voxel[0] + stepX)*_bin_size; // correct
  float next_voxel_boundary_y = (current_voxel[1] + stepY)*_bin_size; // correct
  float next_voxel_boundary_z = (current_voxel[2] + stepZ)*_bin_size; // correct

                                                                       // tMaxX, tMaxY, tMaxZ -- distance until next intersection with voxel-border
                                                                       // the value of t at which the ray crosses the first vertical voxel boundary
  float tMaxX = (ray[0] != 0) ? (next_voxel_boundary_x - ray_start[0]) / ray[0] : FLT_MAX; //
  float tMaxY = (ray[1] != 0) ? (next_voxel_boundary_y - ray_start[1]) / ray[1] : FLT_MAX; //
  float tMaxZ = (ray[2] != 0) ? (next_voxel_boundary_z - ray_start[2]) / ray[2] : FLT_MAX; //

                                                                                            // tDeltaX, tDeltaY, tDeltaZ --
                                                                                            // how far along the ray we must move for the horizontal component to equal the width of a voxel
                                                                                            // the direction in which we traverse the grid
                                                                                            // can only be FLT_MAX if we never go in that direction
  float tDeltaX = (ray[0] != 0) ? _bin_size / ray[0] * stepX : FLT_MAX;
  float tDeltaY = (ray[1] != 0) ? _bin_size / ray[1] * stepY : FLT_MAX;
  float tDeltaZ = (ray[2] != 0) ? _bin_size / ray[2] * stepZ : FLT_MAX;

  Magnum::Vector3i diff(0, 0, 0);
  bool neg_ray = false;
  if (current_voxel[0] != last_voxel[0] && ray[0] < 0) { diff[0]--; neg_ray = true; }
  if (current_voxel[1] != last_voxel[1] && ray[1] < 0) { diff[1]--; neg_ray = true; }
  if (current_voxel[2] != last_voxel[2] && ray[2] < 0) { diff[2]--; neg_ray = true; }
  visited_voxels.push_back(current_voxel);
  if (neg_ray) {
    current_voxel += diff;
    visited_voxels.push_back(current_voxel);
  }

  while (last_voxel != current_voxel) {
    if (tMaxX < tMaxY) {
      if (tMaxX < tMaxZ) {
        current_voxel[0] += static_cast<Magnum::Int>(stepX);
        tMaxX += tDeltaX;
      }
      else {
        current_voxel[2] += static_cast<Magnum::Int>(stepZ);
        tMaxZ += tDeltaZ;
      }
    }
    else {
      if (tMaxY < tMaxZ) {
        current_voxel[1] += static_cast<Magnum::Int>(stepY);
        tMaxY += tDeltaY;
      }
      else {
        current_voxel[2] += static_cast<Magnum::Int>(stepZ);
        tMaxZ += tDeltaZ;
      }
    }
    visited_voxels.push_back(current_voxel);
  }
  return visited_voxels;
}