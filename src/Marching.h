#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include <Magnum\Magnum.h>
#include <Magnum\Math\Vector3.h>
#include <Tesselator.h>
#include <tools\CoordSystem.h>
#include "TesselatorFlatBlock.h"

class MarchingCubes
{
public:

    typedef struct {
       Magnum::Vector3 p[8];
       double val[8];
    } GRIDCELL;

    static void Polygonise(GRIDCELL grid, double isolevel, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, const Magnum::Range2D & uv);

    static Magnum::Vector3 VertexInterp(double isolevel, const Magnum::Vector3 &p1, const Magnum::Vector3 &p2, double valp1, double valp2);

    static void generate(const TesselatorFlatBlock::Data &microblock_data, std::vector<TesselatorVertex> &vertex, std::vector<Magnum::UnsignedInt> &index, Magnum::UnsignedInt &last_index, const WPos &pos, const Magnum::Range2D & uv, SideFlags side = SideFlags::ALL);
};

#endif // MARCHINGCUBES_H