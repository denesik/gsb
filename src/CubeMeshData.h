#pragma once
#ifndef CubeMeshData_h__
#define CubeMeshData_h__

#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Vector2.h>

enum VertexPos
{
  FLB,
  FRB,
  FRT,
  FLT,
  BLB,
  BRB,
  BRT,
  BLT,
};

static const Magnum::Vector3 gVertexCube[] =
{
  { 1.0f, 0.0f, 0.0f },// FRONT LB
  { 0.0f, 0.0f, 0.0f },// FRONT RB
  { 0.0f, 1.0f, 0.0f },// FRONT RT
  { 1.0f, 1.0f, 0.0f },// FRONT LT
  { 1.0f, 0.0f, 1.0f },// BACK LB
  { 0.0f, 0.0f, 1.0f },// BACK RB
  { 0.0f, 1.0f, 1.0f },// BACK RT
  { 1.0f, 1.0f, 1.0f },// BACK LT
};

static const Magnum::Vector3 gVertexData[] =
{
  gVertexCube[FLB],gVertexCube[FRB],gVertexCube[FRT],gVertexCube[FLT], // front
  gVertexCube[FRB],gVertexCube[BRB],gVertexCube[BRT],gVertexCube[FRT], // right
  gVertexCube[BRB],gVertexCube[BLB],gVertexCube[BLT],gVertexCube[BRT], // back
  gVertexCube[BLB],gVertexCube[FLB],gVertexCube[FLT],gVertexCube[BLT], // left
  gVertexCube[FLT],gVertexCube[FRT],gVertexCube[BRT],gVertexCube[BLT], // top
  gVertexCube[BLB],gVertexCube[BRB],gVertexCube[FRB],gVertexCube[FLB], // bot
};

static const Magnum::Vector2 gTextureSide[] =
{
  { 0.0f, 0.0f },
  { 1.0f, 0.0f },
  { 1.0f, 1.0f },
  { 0.0f, 1.0f },
};

static Magnum::UnsignedInt gIndexSide[] =
{
  0, 1, 2, 2, 3, 0
};

#endif // CubeMeshData_h__
