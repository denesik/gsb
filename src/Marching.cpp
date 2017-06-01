#include "Marching.h"

void MarchingCubes::Polygonise(GRIDCELL grid, double isolevel, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, const Magnum::Range2D & uv)
{
  #include "marchinghelper.h"

  int cubeindex = 0;
  if (grid.val[0] < isolevel) cubeindex |= 1;
  if (grid.val[1] < isolevel) cubeindex |= 2;
  if (grid.val[2] < isolevel) cubeindex |= 4;
  if (grid.val[3] < isolevel) cubeindex |= 8;
  if (grid.val[4] < isolevel) cubeindex |= 16;
  if (grid.val[5] < isolevel) cubeindex |= 32;
  if (grid.val[6] < isolevel) cubeindex |= 64;
  if (grid.val[7] < isolevel) cubeindex |= 128;

  /* Cube is entirely in/out of the surface */
  if (edgeTable[cubeindex] == 0)
    return;

  Magnum::Vector3 vertlist[12];

  /* Find the vertices where the surface intersects the cube */
  if (edgeTable[cubeindex] & 1)
    vertlist[0] = VertexInterp(isolevel, grid.p[0], grid.p[1], grid.val[0], grid.val[1]);
  if (edgeTable[cubeindex] & 2)
    vertlist[1] = VertexInterp(isolevel, grid.p[1], grid.p[2], grid.val[1], grid.val[2]);
  if (edgeTable[cubeindex] & 4)
    vertlist[2] = VertexInterp(isolevel, grid.p[2], grid.p[3], grid.val[2], grid.val[3]);
  if (edgeTable[cubeindex] & 8)
    vertlist[3] = VertexInterp(isolevel, grid.p[3], grid.p[0], grid.val[3], grid.val[0]);
  if (edgeTable[cubeindex] & 16)
    vertlist[4] = VertexInterp(isolevel, grid.p[4], grid.p[5], grid.val[4], grid.val[5]);
  if (edgeTable[cubeindex] & 32)
    vertlist[5] = VertexInterp(isolevel, grid.p[5], grid.p[6], grid.val[5], grid.val[6]);
  if (edgeTable[cubeindex] & 64)
    vertlist[6] = VertexInterp(isolevel, grid.p[6], grid.p[7], grid.val[6], grid.val[7]);
  if (edgeTable[cubeindex] & 128)
    vertlist[7] = VertexInterp(isolevel, grid.p[7], grid.p[4], grid.val[7], grid.val[4]);
  if (edgeTable[cubeindex] & 256)
    vertlist[8] = VertexInterp(isolevel, grid.p[0], grid.p[4], grid.val[0], grid.val[4]);
  if (edgeTable[cubeindex] & 512)
    vertlist[9] = VertexInterp(isolevel, grid.p[1], grid.p[5], grid.val[1], grid.val[5]);
  if (edgeTable[cubeindex] & 1024)
    vertlist[10] = VertexInterp(isolevel, grid.p[2], grid.p[6], grid.val[2], grid.val[6]);
  if (edgeTable[cubeindex] & 2048)
    vertlist[11] = VertexInterp(isolevel, grid.p[3], grid.p[7], grid.val[3], grid.val[7]);

  for (size_t i = 0; triTable[cubeindex][i] != -1; i += 3) 
  {
    TesselatorVertex v1;
    v1.vertex = vertlist[triTable[cubeindex][i]] / 3.f + pos;
    v1.textcoord = uv.topLeft();

    TesselatorVertex v2;
    v2.vertex = vertlist[triTable[cubeindex][i + 1]] / 3.f + pos;
    v2.textcoord = uv.bottomLeft();

    TesselatorVertex v3;
    v3.vertex = vertlist[triTable[cubeindex][i + 2]] / 3.f + pos;
    v3.textcoord = uv.bottomRight();

    Magnum::Vector3 N = Magnum::Math::cross((v1.vertex - v2.vertex), (v2.vertex - v3.vertex));

    if (N.length() > 0)
    {
      N = -N.normalized();
    }

    v1.normal = v1.normal = v1.normal = N;

    vertex.push_back(v1);
    vertex.push_back(v2);
    vertex.push_back(v3);

    index.push_back(last_index + 0);
    index.push_back(last_index + 1);
    index.push_back(last_index + 2);

    last_index += 3;
  }
}

Magnum::Vector3 MarchingCubes::VertexInterp(double isolevel, const Magnum::Vector3 & p1, const Magnum::Vector3 & p2, double valp1, double valp2)
{
    double mu;
    Magnum::Vector3 p;

    if (abs(isolevel - valp1) < 0.00001)
        return(p1);
    if (abs(isolevel - valp2) < 0.00001)
        return(p2);
    if (abs(valp1 - valp2) < 0.00001)
        return(p1);
    mu = (isolevel - valp1) / (valp2 - valp1);
    p.x() = p1.x() + mu * (p2.x() - p1.x());
    p.y() = p1.y() + mu * (p2.y() - p1.y());
    p.z() = p1.z() + mu * (p2.z() - p1.z());

    return(p);
}

void MarchingCubes::generate(const TesselatorFlatBlock::Data &microblock_data, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, const Magnum::Range2D & uv, SideFlags side /*= SideFlags::ALL*/)
{
  int size = 4;

  for (int i = 0; i < size - 1; ++i)
    for (int j = 0; j < size - 1; ++j)
      for (int k = 0; k < size - 1; ++k)
      {
        GRIDCELL gc;
        gc.p[0] = { static_cast<float>(i),     static_cast<float>(j + 1), static_cast<float>(k)     };
        gc.p[1] = { static_cast<float>(i + 1), static_cast<float>(j + 1), static_cast<float>(k)     };
        gc.p[2] = { static_cast<float>(i + 1), static_cast<float>(j + 1), static_cast<float>(k + 1) };
        gc.p[3] = { static_cast<float>(i),     static_cast<float>(j + 1), static_cast<float>(k + 1) };
        gc.p[4] = { static_cast<float>(i),     static_cast<float>(j),     static_cast<float>(k)     };
        gc.p[5] = { static_cast<float>(i + 1), static_cast<float>(j),     static_cast<float>(k)     };
        gc.p[6] = { static_cast<float>(i + 1), static_cast<float>(j),     static_cast<float>(k + 1) };
        gc.p[7] = { static_cast<float>(i),     static_cast<float>(j),     static_cast<float>(k + 1) };
        for (int mm = 0; mm < 8; mm++)
        {
          gc.val[mm] = microblock_data.get()[TesselatorFlatBlock::ToIndex({
            static_cast<int>(gc.p[mm].x()), 
            static_cast<int>(gc.p[mm].y()), 
            static_cast<int>(gc.p[mm].z())
          }, size)] ? 255 : 0;

          gc.p[mm] *= 1;
        }
        Polygonise(gc, 1, vertex, index, last_index, pos, uv);
      }
}
