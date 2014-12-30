/*
 * This file is part of DDAD.
 *
 * DDAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * DDAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with DDAD. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GE_TERRAIN_H
#define GE_TERRAIN_H

#include "common.h"
#include "visual.h"
#include "quadedge.h"
#include "matrix.h"
#include "point.h"
#include "pointset.h"
#include "predicate.h"
#include "aabb.h"

using namespace DDAD::Visual;

namespace DDAD {

//=============================================================================
// Interface: RegionalTerrain_3r
//=============================================================================

/*!
 * RegionalTerrain_3r supports creating 3D terrain meshes from a set of known
 * sample points. It does not support adding arbitrary sample points that lie
 * outside the bounding rectangle. This is due to the nature of incremental
 * Delaunay triangulation algorithms.
 */
class RegionalTerrain_3r : public Visual::Geometry {
public:
    RegionalTerrain_3r();
    ~RegionalTerrain_3r();

    void Initialize(const AABB_2r& region);
    void AddSample(const Point_3r& sample);

private:

    // visualization helper functions
    void SigPushVertex(QuadEdge::Vertex* v);
    void SigPopVertex(QuadEdge::Vertex* v);
    void SigPushEdge(QuadEdge::Edge* e);
    void SigPopEdge(QuadEdge::Edge* e);
    void SigPushFace(QuadEdge::Face* f);
    void SigPopFace(QuadEdge::Face* f);

    QuadEdge::Edge* MakeVertexEdge(QuadEdge::Vertex *v,
                                   QuadEdge::Face *left,
                                   QuadEdge::Face *right,
                                   SharedPoint_3r vnew_pos);

    QuadEdge::Edge* MakeFaceEdge(QuadEdge::Face *f,
                                 QuadEdge::Vertex *org,
                                 QuadEdge::Vertex *dest);

    void KillVertexEdge(QuadEdge::Edge *e);
    void KillFaceEdge(QuadEdge::Edge *e);

    size_t EdgeCount(QuadEdge::Face *f);

    // delaunay triangulation subroutines
    QuadEdge::Edge* LocalizePoint(const Point_3r& sample);
    void TestAndSwapEdges(std::vector<QuadEdge::Edge*>& edges,
                          const Point_3r& sample);


    QuadEdge::Cell* terrain_;
    AABB_2r region_;

    Material mat_vertex_;
    Material mat_edge_;
    Material mat_face_;
};

RegionalTerrain_3r DelaunayTerrain(const PointSet_3r&, IGeometryObserver* obs);

} // namespace DDAD

#endif // GE_TERRAI_H
