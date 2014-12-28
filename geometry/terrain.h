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

using namespace DDAD::Visual;

namespace DDAD {

class AABB_2r {
public:
    AABB_2r() {}

    AABB_2r(const PointSet_3r& pointset) {
        rational minx, miny, maxx, maxy;
        minx = maxx = pointset.points()[0]->x();
        miny = maxy = pointset.points()[0]->y();

        for (auto point : pointset.points()) {
            minx = std::min(point->x(), minx);
            miny = std::min(point->y(), miny);
            maxx = std::max(point->x(), maxx);
            maxy = std::max(point->y(), maxy);
        }

        min_.set_x(minx);
        min_.set_y(miny);
        max_.set_x(maxx);
        max_.set_y(maxy);
    }

    const Point_2r& min() const {
        return min_;
    }

    const Point_2r& max() const {
        return max_;
    }

private:
    Point_2r min_;
    Point_2r max_;
};

//=============================================================================
// Interface: RegionalTerrain_3r
//=============================================================================

/*!
 * The purpose of the class is to support the creation of 3D terrain meshes
 * from a set of known sample points. The class does not support adding
 * arbitrary sample points that lie outside the bounding rectangle. This is
 * due largely to the nature of incremental delaunay triangulation algorithms.
 * The class supports the use of free functions as algorithms as follows:
 *
 * Initialization() takes a bounding rectangle for the input point set
 * AddPoint() takes a sample point that lies within the bounding rectangle

 * Thus, our incremental delaunay algorithm can be written following the normal
 * convention of:
 *
 *  Terrain_3r Delaunay(points, observer) {
 *      Terrain_3r out;
 *      out.AddObserver(observer);
 *
 *      out.Initialize(bounding rect)
 *
 *      for (point in points) {
 *          out.AddSample(point);
 *      }
 *
 *      return out;
 *  }
 *
 * We might name the class RegionalTerrain_3r. We could support this class as
 * a first-class member of our system by allowing a user to create
 * RegionalTerrains by dragging out the bounding rectangle, then adding points
 * inside and adjusting their height.
 */
class RegionalTerrain_3r : public Visual::Geometry {
public:
    RegionalTerrain_3r();
    ~RegionalTerrain_3r();

    void Initialize(const AABB_2r& region);
    void AddSample(const Point_3r& sample);

private:

    // visualization helper functions
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

//=============================================================================
// Implementation: DelaunayTerrain
//=============================================================================

inline RegionalTerrain_3r DelaunayTerrain(const PointSet_3r& samples,
                                          IGeometryObserver* obs) {
    RegionalTerrain_3r terrain;
    terrain.AddObserver(obs);

    terrain.Initialize(AABB_2r(samples));

    for (auto sample : samples.points()) {
        terrain.AddSample(*sample);
    }

    return terrain;
}

//=============================================================================
// Implementation: RegionalTerrain_3r
//=============================================================================

inline RegionalTerrain_3r::RegionalTerrain_3r() :
    mat_face_(Material(Color::GRAY, Color::GRAY, Color::GRAY)) {
    LOG(DEBUG) << "constructing terrain";
}

inline RegionalTerrain_3r::~RegionalTerrain_3r() {
    LOG(DEBUG) << "destroying terrain";
}

// Triangulation Methods ======================================================

inline void RegionalTerrain_3r::Initialize(const AABB_2r& region) {
    LOG(DEBUG) << "initializing terrain";

    region_ = region;

    // setup bbox topology
    terrain_ = QuadEdge::Cell::make();

    // grab the initial vertex
    QuadEdge::CellVertexIterator iter(terrain_);
    QuadEdge::Vertex *v1 = iter.next();

    // grab left face (inside) and right face (outside)
    QuadEdge::Face *left = v1->getEdge()->Left();
    QuadEdge::Face *right = v1->getEdge()->Right();

    // create 3 new vertices along the initial edge
    QuadEdge::Vertex *v2 = terrain_->makeVertexEdge(v1, left, right)->Dest();
    QuadEdge::Vertex *v3 = terrain_->makeVertexEdge(v2, left, right)->Dest();
    QuadEdge::Vertex *v4 = terrain_->makeVertexEdge(v3, left, right)->Dest();

    // set bbox vertex positions ccw
    v1->pos = std::make_shared<Point_3r>(region.min().x()-1, region.min().y()-1, 0);
    v2->pos = std::make_shared<Point_3r>(region.max().x()+1, region.min().y()-1, 0);
    v3->pos = std::make_shared<Point_3r>(region.max().x()+1, region.max().y()+1, 0);
    v4->pos = std::make_shared<Point_3r>(region.min().x()-1, region.max().y()+1, 0);

    // draw vertices
    QuadEdge::CellVertexIterator terrain_verts(terrain_);
    QuadEdge::Vertex *v;
    while ((v = terrain_verts.next()) != 0) {
        SigRegisterPoint_3r(*v->pos);
        SigPushVisualPoint_3r(*v->pos, Visual::Point(mat_vertex_));
    }

    // draw faces and edges
    QuadEdge::CellFaceIterator terrain_faces(terrain_);
    QuadEdge::Face *f;
    while ((f = terrain_faces.next()) != 0) {
        SigPushFace(f);

        QuadEdge::FaceEdgeIterator face_edges(f);
        QuadEdge::Edge *e;
        while ((e = face_edges.next()) != 0) {
            SigPushEdge(e);
        }
    }

    // add edge across diagonal
    MakeFaceEdge(left, v1, v3);
}

inline void RegionalTerrain_3r::AddSample(const Point_3r& sample) {
    SharedPoint_3r sample_r = std::make_shared<Point_3r>(
        sample.x(), sample.y(), sample.z()
    );
    SigRegisterPoint_3r(*sample_r);

    // locate point
    QuadEdge::Edge *e0;
    QuadEdge::CellFaceIterator fitr(terrain_);
    e0 = fitr.next()->getEdge();
    QuadEdge::Edge *e1 = LocalizePoint(*sample_r);

    QuadEdge::Vertex *v1 = e1->Org();
    QuadEdge::Vertex *v2 = e1->Dest();
    QuadEdge::Face *f = e1->Left();
    QuadEdge::Edge *e2 = e1->Lnext();
    QuadEdge::Edge *e3 = e1->Lprev();
    QuadEdge::Vertex *v3 = e2->Dest();

    QuadEdge::Edge *newEdge1 = MakeFaceEdge(f, v1, v2);
    QuadEdge::Face *f2 = newEdge1->Right();

    QuadEdge::Vertex *vnew = MakeVertexEdge(v2, f2, f, sample_r)->Dest();
    MakeFaceEdge(f, vnew, v3);

    std::vector<QuadEdge::Edge*> neighbors;
    neighbors.push_back(e1->Sym());
    neighbors.push_back(e2->Sym());
    neighbors.push_back(e3->Sym());
    TestAndSwapEdges(neighbors, *sample_r);
}

inline QuadEdge::Edge* RegionalTerrain_3r::LocalizePoint(const Point_3r& sample) {
    QuadEdge::CellFaceIterator faces(terrain_);
    QuadEdge::Face *f = nullptr;
    while ((f = faces.next())) {
        QuadEdge::Edge *e1 = f->getEdge();
        QuadEdge::Edge *e2 = e1->Lnext();
        QuadEdge::Vertex *v1 = e1->Org();
        QuadEdge::Vertex *v2 = e1->Dest();
        QuadEdge::Vertex *v3 = e2->Dest();
        if (Predicate::Orient2D(*v1->pos, *v2->pos, sample) >= 0 &&
            Predicate::Orient2D(*v2->pos, *v3->pos, sample) >= 0 &&
            Predicate::Orient2D(*v3->pos, *v1->pos, sample) >= 0) {
            return e1;
        }
    }

    return nullptr;
}

inline void RegionalTerrain_3r::TestAndSwapEdges(std::vector<QuadEdge::Edge*>& edges,
                                                 const Point_3r& sample) {
    while (!edges.empty()) {
        QuadEdge::Edge *e1 = edges.back();
        edges.pop_back();
        QuadEdge::Vertex *v1 = e1->Org(), *v2 = e1->Dest();
        QuadEdge::Edge *e2 = e1->Lnext();
        QuadEdge::Edge *e3 = e1->Lprev();
        QuadEdge::Vertex *v3 = e2->Dest();
        QuadEdge::Edge *e4 = e1->Rnext();
        QuadEdge::Edge *e5 = e1->Rprev();

        if (Predicate::InCircle(*v1->pos, *v2->pos, *v3->pos, sample) > 0) {
            QuadEdge::Face *left = e1->Left();
            KillFaceEdge(e1);
            MakeFaceEdge(left, e2->Dest(), e5->Dest());
            edges.push_back(e2->Sym());
            edges.push_back(e3->Sym());
        }
    }
}

// Visualization Methods ======================================================

inline void RegionalTerrain_3r::SigPushEdge(QuadEdge::Edge* e) {
    Segment_3r s(e->Org()->pos, e->Dest()->pos);
    SigPushVisualSegment_3r(s, Visual::Segment(mat_edge_), 1000);
}

inline void RegionalTerrain_3r::SigPopEdge(QuadEdge::Edge* e) {
    Segment_3r s(e->Org()->pos, e->Dest()->pos);
    SigPopVisualSegment_3r(s);
}

inline void RegionalTerrain_3r::SigPushFace(QuadEdge::Face* f) {
    QuadEdge::FaceEdgeIterator faceEdges(f);
    QuadEdge::Edge *e;

    e = faceEdges.next();
    auto fan_pivot = e->Org()->pos;
    e = faceEdges.next();
    auto fan_middle = e->Org()->pos;
    e = faceEdges.next();
    auto fan_last = e->Org()->pos;
    Triangle_3r tri0(fan_pivot, fan_middle, fan_last);
    SigPushVisualTriangle_3r(tri0, Visual::Triangle(mat_face_), 1000);
    while ((e = faceEdges.next()) != 0) {
        fan_middle = fan_last;
        fan_last = e->Org()->pos;
        Triangle_3r tri(fan_pivot, fan_middle, fan_last);
        SigPushVisualTriangle_3r(tri, Visual::Triangle(mat_face_), 1000);
    }
}

inline void RegionalTerrain_3r::SigPopFace(QuadEdge::Face* f) {
    QuadEdge::FaceEdgeIterator faceEdges(f);
    QuadEdge::Edge *e;

    e = faceEdges.next();
    auto fan_pivot = e->Org()->pos;
    e = faceEdges.next();
    auto fan_middle = e->Org()->pos;
    e = faceEdges.next();
    auto fan_last = e->Org()->pos;
    Triangle_3r tri0(fan_pivot, fan_middle, fan_last);
    SigPopVisualTriangle_3r(tri0);
    while ((e = faceEdges.next()) != 0) {
        fan_middle = fan_last;
        fan_last = e->Org()->pos;
        Triangle_3r tri(fan_pivot, fan_middle, fan_last);
        SigPopVisualTriangle_3r(tri);
    }
}

inline QuadEdge::Edge* RegionalTerrain_3r::MakeVertexEdge(QuadEdge::Vertex *v,
                                                          QuadEdge::Face *left,
                                                          QuadEdge::Face *right,
                                                          SharedPoint_3r vnew_pos) {
    SigPopFace(left);
    SigPopFace(right);
    QuadEdge::Edge* e = terrain_->makeVertexEdge(v, left, right);
    QuadEdge::Vertex* vnew = e->Dest();
    vnew->pos = vnew_pos;
    // assume vpos is registered
    SigPushVisualPoint_3r(*vnew->pos, Visual::Point(mat_vertex_));
    SigPushEdge(e);
    SigPushEdge(e->Sym());
    return e;
}

inline QuadEdge::Edge* RegionalTerrain_3r::MakeFaceEdge(QuadEdge::Face *f,
                                                        QuadEdge::Vertex *org,
                                                        QuadEdge::Vertex *dest) {
    SigPopFace(f);
    QuadEdge::Edge* e = terrain_->makeFaceEdge(f, org, dest);
    SigPushFace(e->Left());
    SigPushFace(e->Right());
    SigPushEdge(e);
    SigPushEdge(e->Sym());
    return e;
}

inline void RegionalTerrain_3r::KillVertexEdge(QuadEdge::Edge *e) {
    QuadEdge::Face* left = e->Left();
    QuadEdge::Face* right = e->Right();
    SigPopFace(left);
    SigPopFace(right);
    SigPopEdge(e);
    SigPopEdge(e->Sym());
    SigPopVisualPoint_3r(*e->Dest()->pos);
    terrain_->killVertexEdge(e);
    SigPushFace(left);
    SigPushFace(right);
}

inline void RegionalTerrain_3r::KillFaceEdge(QuadEdge::Edge *e) {
    QuadEdge::Face* left = e->Left();
    QuadEdge::Face* right = e->Right();
    SigPopFace(left);
    SigPopFace(right);
    SigPopEdge(e);
    SigPopEdge(e->Sym());
    terrain_->killFaceEdge(e);
    SigPushFace(left);
}



/*
 * there is the question of whether to push only one directed edge or both
 * directed edges. this is complicated by the fact that the edge iterator
 * construction visits edges twice. it is easier to push both.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */










class TerrainMesh_3r : public Visual::Geometry {
public:
    TerrainMesh_3r() {
        Visual::Color blue(0, 151, 255, 255);
        Visual::Material blue_mat;
        blue_mat.set_ambient(blue);

        viz_point_.set_material(blue_mat);
        viz_segment_.set_material(blue_mat);
        viz_triangle_.set_material(blue_mat);
    }

    void Initialize(const std::vector<Point_3f>& samples) {
        assert(!samples.empty());

        // setup bounding triangle topology
        cell_ = QuadEdge::Cell::make();

        QuadEdge::CellVertexIterator iter(cell_);
        QuadEdge::Vertex *v1 = iter.next();

        QuadEdge::Edge *a = v1->getEdge();
        QuadEdge::Face *left = a->Left();
        QuadEdge::Face *right = a->Right();

        QuadEdge::Vertex *v2 = cell_->makeVertexEdge(v1, left, right)->Dest();
        QuadEdge::Vertex *v3 = cell_->makeVertexEdge(v2, left, right)->Dest();

        // compute a bounding box of the input points xy coordinates
        float maxx, maxy, maxz, minx, miny, minz;
        minx = maxx = samples.at(0).x();
        miny = maxy = samples.at(0).y();
        minz = maxz = samples.at(0).z();
        for (auto i = begin(samples); i != end(samples); ++i) {
            minx = std::min(i->x(), minx);
            miny = std::min(i->y(), miny);
            minz = std::min(i->z(), minz);
            maxx = std::max(i->x(), maxx);
            maxy = std::max(i->y(), maxy);
            maxz = std::max(i->z(), maxz);
        }

        // compute the max variance along a cardinal axis
        float dx = maxx - minx;
        float dy = maxy - miny;
        float dmax = std::max(dx, dy);
        dmax = std::max(dmax, 10.0f);   // in case of a single point
        float cx = dx * 0.5f;
        float cy = dy * 0.5f;

        // set triangle vertex positions
        v1->pos = std::make_shared<Point_3r>(cx - 20.0f * dmax, cy - dmax, 0);
        SigRegisterPoint_3r(*v1->pos);
        v2->pos = std::make_shared<Point_3r>(cx + 20.0f * dmax, cy - dmax, 0);
        SigRegisterPoint_3r(*v2->pos);
        v3->pos = std::make_shared<Point_3r>(cx, cy + 20.0f * dmax, 0);
        SigRegisterPoint_3r(*v3->pos);


        dummy_ = std::make_shared<Point_3r>(0, 0, 0);
        SigRegisterPoint_3r(*dummy_);
        ConstructViz();

        // incrementally construct the triangulation
        for (auto i = begin(samples); i != end(samples); ++i) {
            ClearViz();
            AddPoint(*i);
            ConstructViz();
        }

        //ClearViz();
        //DeleteSetupVertices(cell_, v1, v2, v3);
        //ConstructViz();
    }

    SharedPoint_3r dummy_;

    void ClearViz() {
        // remove vertices
        QuadEdge::CellVertexIterator cellVerts(cell_);
        QuadEdge::Vertex *v;
        while ((v = cellVerts.next()) != 0) {
            SigPopVisualPoint_3r(*v->pos);
        }

        // remove edges
        QuadEdge::CellFaceIterator cellFaces(cell_);
        QuadEdge::Face *f;
        while ((f = cellFaces.next()) != 0) {
            QuadEdge::FaceEdgeIterator faceEdges(f);
            QuadEdge::Edge *e;
            while ((e = faceEdges.next()) != 0) {
                SigPopDirectedEdge(e);
            }
        }

        // remove faces
        cellFaces = QuadEdge::CellFaceIterator(cell_);
        while ((f = cellFaces.next()) != 0) {
            SigPopFace(f);
        }
    }

    void ConstructViz() {
        // visualize vertices
        QuadEdge::CellVertexIterator cellVerts(cell_);
        QuadEdge::Vertex *v;
        while ((v = cellVerts.next()) != 0) {
            SigPushVisualPoint_3r(*v->pos, viz_point_);
        }

        // visualize edges
        QuadEdge::CellFaceIterator cellFaces(cell_);
        QuadEdge::Face *f;
        while ((f = cellFaces.next()) != 0) {
            QuadEdge::FaceEdgeIterator faceEdges(f);
            QuadEdge::Edge *e;
            while ((e = faceEdges.next()) != 0) {
                SigPushDirectedEdge(e);
            }
        }

        // visualize faces
        cellFaces = QuadEdge::CellFaceIterator(cell_);
        while ((f = cellFaces.next()) != 0) {
            SigPushFace(f);
        }

        SigPushVisualPoint_3r(*dummy_, viz_point_);
        SigPopVisualPoint_3r(*dummy_, 1000);

        /*
        cellFaces = QuadEdge::CellFaceIterator(cell_);
        f = cellFaces.next();
        SigPopFace(f);
        SigPushFace(f);
        */
    }

    void SigPushFace(QuadEdge::Face* f) {
        QuadEdge::FaceEdgeIterator faceEdges(f);
        QuadEdge::Edge *e;

        e = faceEdges.next();
        auto fan_pivot = e->Org()->pos;
        e = faceEdges.next();
        auto fan_middle = e->Org()->pos;
        e = faceEdges.next();
        auto fan_last = e->Org()->pos;
        Triangle_3r tri0(fan_pivot, fan_middle, fan_last);
        SigPushVisualTriangle_3r(tri0, viz_triangle_);
        while ((e = faceEdges.next()) != 0) {
            fan_middle = fan_last;
            fan_last = e->Org()->pos;
            Triangle_3r tri(fan_pivot, fan_middle, fan_last);
            SigPushVisualTriangle_3r(tri, viz_triangle_);
        }
    }

    void SigPopFace(QuadEdge::Face* f) {
        QuadEdge::FaceEdgeIterator faceEdges(f);
        QuadEdge::Edge *e;

        e = faceEdges.next();
        auto fan_pivot = e->Org()->pos;
        e = faceEdges.next();
        auto fan_middle = e->Org()->pos;
        e = faceEdges.next();
        auto fan_last = e->Org()->pos;
        Triangle_3r tri0(fan_pivot, fan_middle, fan_last);
        SigPopVisualTriangle_3r(tri0);
        while ((e = faceEdges.next()) != 0) {
            fan_middle = fan_last;
            fan_last = e->Org()->pos;
            Triangle_3r tri(fan_pivot, fan_middle, fan_last);
            SigPopVisualTriangle_3r(tri);
        }
    }

    void SigPushDirectedEdge(QuadEdge::Edge* e) {
        Segment_3r s(e->Org()->pos, e->Dest()->pos);
        if (e->Org() < e->Dest()) {
            SigPushVisualSegment_3r(s, viz_segment_);
        }
    }

    void SigPopDirectedEdge(QuadEdge::Edge* e) {
        Segment_3r s(e->Org()->pos, e->Dest()->pos);
        if (e->Org() < e->Dest()) {
            SigPopVisualSegment_3r(s);
        }
    }

    // deletes the vertex at the origin of the given edge
    void DeleteVertexFromTriangulation(QuadEdge::Cell *c,
                                       QuadEdge::Edge *startEdge) {
        // the next line (commented) is only there for understanding
        //Vertex *vertexToDelete = startEdge->Org();
        QuadEdge::Edge *endEdge = startEdge->Oprev();
        QuadEdge::Edge *currentEdge = startEdge;
        QuadEdge::Vertex *lastVertex = NULL;
    //    Face *f = currentEdge->Left();
        while (currentEdge != endEdge) {
            // save values
            QuadEdge::Edge *next = currentEdge->Onext();
            QuadEdge::Vertex *nextDest = next->Dest();
            QuadEdge::Vertex *dest = currentEdge->Dest();

            // delete the current edge
            c->killFaceEdge(currentEdge);

            // fix the dest vertices' triangulation
            // to change to deleting internal points, fix this section
            if (lastVertex != NULL) {
                // if a turn to the left, then dest is not on the convex hull and
                // the triangulation needs a triangle to fix the convex hull
                if (Predicate::Orient2D(*lastVertex->pos,
                                        *dest->pos,
                                        *nextDest->pos) > 0) {
                    c->makeFaceEdge(next->Right(), nextDest, lastVertex);
                }
            }

            // use saved values
            currentEdge = next;
            lastVertex = dest;
        }
        // only endEdge is left of the edges containing vertexToDelete
        // this line also should effectively delete vertexToDelete
        c->killVertexEdge(endEdge->Sym());
    }

    // deletes the vertices for the initial face created in setUpInitialCell,
    // leaving the result of the Delauney triangulation of the input points
    // as the remainder
    // This function assumes that the three vertices passed in have edges between
    // them and a face that is made up of only these edges (infinite face), so that
    // the selections of edges to do deletions from works correctly
    void DeleteSetupVertices(QuadEdge::Cell *c,
                             QuadEdge::Vertex *initialVertex1,
                             QuadEdge::Vertex *initialVertex2,
                             QuadEdge::Vertex *initialVertex3) {
        QuadEdge::VertexEdgeIterator vei(initialVertex1);
        QuadEdge::Edge *e = NULL;
        while (e == NULL || e->Dest() != initialVertex2) {
            e = vei.next();
        }
        if (e->Onext()->Dest() == initialVertex3) {
            e = e->Onext();
        }
        QuadEdge::Edge *e1 = e;
        QuadEdge::Edge *e2 = e1->Sym()->Onext();
        QuadEdge::Edge *e3 = e2->Sym()->Onext()->Onext();
        DeleteVertexFromTriangulation(c, e1);
        DeleteVertexFromTriangulation(c, e2);
        DeleteVertexFromTriangulation(c, e3);
    }

    void AddPoint(const Point_3f& sample) {
        SharedPoint_3r sample_r = std::make_shared<Point_3r>(
            sample.x(), sample.y(), sample.z()
        );
        SigRegisterPoint_3r(*sample_r);

        // locate point
        QuadEdge::Edge *e0;
        QuadEdge::CellFaceIterator fitr(cell_);
        e0 = fitr.next()->getEdge();
        QuadEdge::Edge *e1 = LocalizePoint(*sample_r);

        QuadEdge::Vertex *v1 = e1->Org();
        QuadEdge::Vertex *v2 = e1->Dest();
        QuadEdge::Face *f = e1->Left();
        QuadEdge::Edge *e2 = e1->Lnext();
        QuadEdge::Edge *e3 = e1->Lprev();
        QuadEdge::Vertex *v3 = e2->Dest();
        QuadEdge::Edge *newEdge1 = cell_->makeFaceEdge(f, v1, v2);
        QuadEdge::Face *f2 = newEdge1->Right();
        QuadEdge::Vertex *vnew = cell_->makeVertexEdge(v2, f2, f)->Dest();
        vnew->pos = sample_r;
        cell_->makeFaceEdge(f, vnew, v3);

        std::vector<QuadEdge::Edge*> neighbors;
        neighbors.push_back(e1->Sym());
        neighbors.push_back(e2->Sym());
        neighbors.push_back(e3->Sym());
        TestAndSwapEdges(neighbors, *sample_r);
    }

    QuadEdge::Edge* LocalizePoint(const Point_3r& sample) {
        QuadEdge::CellFaceIterator faces(cell_);
        QuadEdge::Face *f = NULL;

        while ((f = faces.next())) {
            QuadEdge::Edge *e1 = f->getEdge();
            QuadEdge::Edge *e2 = e1->Lnext();
            QuadEdge::Vertex *v1 = e1->Org();
            QuadEdge::Vertex *v2 = e1->Dest();
            QuadEdge::Vertex *v3 = e2->Dest();
            if (Predicate::Orient2D(*v1->pos, *v2->pos, sample) >= 0 &&
                Predicate::Orient2D(*v2->pos, *v3->pos, sample) >= 0 &&
                Predicate::Orient2D(*v3->pos, *v1->pos, sample) >= 0) {
                return e1;
            }
        }

        return nullptr;
    }

    void TestAndSwapEdges(std::vector<QuadEdge::Edge*>& edges,
                          const Point_3r& sample) {
        while (!edges.empty()) {
            QuadEdge::Edge *e1 = edges.back();
            edges.pop_back();
            QuadEdge::Vertex *v1 = e1->Org(), *v2 = e1->Dest();
            QuadEdge::Edge *e2 = e1->Lnext();
            QuadEdge::Edge *e3 = e1->Lprev();
            QuadEdge::Vertex *v3 = e2->Dest();
            QuadEdge::Edge *e4 = e1->Rnext();
            QuadEdge::Edge *e5 = e1->Rprev();

            if (Predicate::InCircle(*v1->pos, *v2->pos, *v3->pos, sample) > 0) {
                QuadEdge::Face *left = e1->Left();
                cell_->killFaceEdge(e1);
                cell_->makeFaceEdge(left, e2->Dest(), e5->Dest());
                edges.push_back(e2->Sym());
                edges.push_back(e3->Sym());
            }
        }
    }

private:
    QuadEdge::Cell* cell_;
    Visual::Point viz_point_;
    Visual::Segment viz_segment_;
    Visual::Triangle viz_triangle_;
};

} // namespace DDAD

#endif // GE_TRIANGULATION_H
