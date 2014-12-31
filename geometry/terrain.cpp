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

#include "common.h"
#include "terrain.h"

using namespace DDAD::Visual;

namespace DDAD {

//=============================================================================
// Algorithms
//=============================================================================

RegionalTerrain_3r DelaunayTerrain(const PointSet_3r& samples,
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

RegionalTerrain_3r::RegionalTerrain_3r() :
    mat_face_(Material(Color::GRAY, Color::GRAY, Color::GRAY,
                       Coverage::E_OPAQUE, Lighting::E_FLAT)) {
    LOG(DEBUG) << "constructing terrain";
}

RegionalTerrain_3r::~RegionalTerrain_3r() {
    LOG(DEBUG) << "destroying terrain";
}

// Triangulation Methods ======================================================

void RegionalTerrain_3r::Initialize(const AABB_2r& region) {
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
    v1->pos = std::make_shared<Point_3r>(region.min().x() - 1,
                                         region.min().y() - 1, -1);
    v2->pos = std::make_shared<Point_3r>(region.max().x() + 1,
                                         region.min().y() - 1, -1);
    v3->pos = std::make_shared<Point_3r>(region.max().x() + 1,
                                         region.max().y() + 1, -1);
    v4->pos = std::make_shared<Point_3r>(region.min().x() - 1,
                                         region.max().y() + 1, -1);

    // add edge across diagonal
    terrain_->makeFaceEdge(left, v1, v3);

    // draw vertices
    QuadEdge::CellVertexIterator terrain_verts(terrain_);
    QuadEdge::Vertex *v;
    while ((v = terrain_verts.next()) != 0) {
        SigRegisterPoint_3r(*v->pos);
        SigPushVertex(v);
    }

    // draw faces and edges
    QuadEdge::CellFaceIterator terrain_faces(terrain_);
    QuadEdge::Face *f;
    while ((f = terrain_faces.next()) != 0) {
        SigPushFace(f);

        QuadEdge::FaceEdgeIterator face_edges(f);
        QuadEdge::Edge *e;
        while ((e = face_edges.next()) != 0) {
            if (e->Org() < e->Dest()) {
                SigPushEdge(e);
            }
        }
    }
}

void RegionalTerrain_3r::AddSample(const Point_3r& sample) {
    SharedPoint_3r sample_r = std::make_shared<Point_3r>(
        sample.x(), sample.y(), sample.z()-1
    );
    SigRegisterPoint_3r(*sample_r);

    // find the triangle containing the sample
    QuadEdge::Edge *e1 = LocalizePoint(*sample_r);
    QuadEdge::Edge *e2 = e1->Lnext();
    QuadEdge::Edge *e3 = e1->Lprev();
    QuadEdge::Vertex *v1 = e1->Org();
    QuadEdge::Vertex *v2 = e1->Dest();
    QuadEdge::Vertex *v3 = e2->Dest();
    QuadEdge::Face *f = e1->Left();

    // stick the new sample into the containing triangle and update topology
    QuadEdge::Edge *enew1 = MakeFaceEdge(f, v1, v2);
    QuadEdge::Face *f2 = enew1->Right();
    QuadEdge::Vertex *vnew = MakeVertexEdge(v2, f2, f, sample_r)->Dest();
    QuadEdge::Edge *enew2 = MakeFaceEdge(f, vnew, v3);

    std::vector<QuadEdge::Edge*> neighbors;
    neighbors.push_back(e1->Sym());
    neighbors.push_back(e2->Sym());
    neighbors.push_back(e3->Sym());
    TestAndSwapEdges(neighbors, *sample_r);
}

QuadEdge::Edge* RegionalTerrain_3r::LocalizePoint(const Point_3r& sample) {
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

void RegionalTerrain_3r::TestAndSwapEdges(std::vector<QuadEdge::Edge*>& edges,
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

void RegionalTerrain_3r::SigPushVertex(QuadEdge::Vertex *v) {
    SigPushVisualPoint_3r(*v->pos, Visual::Point(mat_vertex_));
}

void RegionalTerrain_3r::SigPopVertex(QuadEdge::Vertex *v) {
    SigPopVisualPoint_3r(*v->pos);
}

void RegionalTerrain_3r::SigPushEdge(QuadEdge::Edge* e) {
    Segment_3r s1(e->Org()->pos, e->Dest()->pos);
    Segment_3r s2(e->Dest()->pos, e->Org()->pos);
    SigPushVisualSegment_3r(s1, Visual::Segment(mat_edge_));
    SigPushVisualSegment_3r(s2, Visual::Segment(mat_edge_), 25);
}

void RegionalTerrain_3r::SigPopEdge(QuadEdge::Edge* e) {
    Segment_3r s1(e->Org()->pos, e->Dest()->pos);
    Segment_3r s2(e->Dest()->pos, e->Org()->pos);
    SigPopVisualSegment_3r(s1);
    SigPopVisualSegment_3r(s2);
}

void RegionalTerrain_3r::SigPushFace(QuadEdge::Face* f) {

    // ignore loop, sliver, and quadrilateral topology
    if (EdgeCount(f) != 3) {
        return;
    }

    QuadEdge::FaceEdgeIterator edges(f);
    auto a = edges.next()->Org()->pos;
    auto b = edges.next()->Org()->pos;
    auto c = edges.next()->Org()->pos;
    SigPushVisualTriangle_3r(Triangle_3r(c, a, b),
                             Visual::Triangle(mat_face_));
}

void RegionalTerrain_3r::SigPopFace(QuadEdge::Face* f) {

    // ignore loop, sliver, and quadrilateral topology
    if (EdgeCount(f) != 3) {
        return;
    }

    QuadEdge::FaceEdgeIterator edges(f);
    auto a = edges.next()->Org()->pos;
    auto b = edges.next()->Org()->pos;
    auto c = edges.next()->Org()->pos;
    SigPopVisualTriangle_3r(Triangle_3r(c, a, b));
}

QuadEdge::Edge* RegionalTerrain_3r::MakeVertexEdge(QuadEdge::Vertex *v,
                                                   QuadEdge::Face *left,
                                                   QuadEdge::Face *right,
                                                   SharedPoint_3r vnew_pos) {

    // we need to pop all faces and edges on the ccw traveral from the left
    // face to the right face in v's orbit. the vertexedgeiterator will begin
    // at a random edge in the orbit, and we have no way of determining whether
    // we are inside or outside the left-to-right range on the first pass
    // through the edges. the iterator will give us the edges back in ccw order
    // however.

    // the strategy is to place all edges in v's orbit into a list and rotate
    // the list so that the leftmost edge to pop is the first element.
    std::list<QuadEdge::Edge*> rotated_orbit;

    // iterate through the orbit and track the leftmost edge insertion index.
    size_t right_idx = 0;
    size_t i = 0;
    QuadEdge::VertexEdgeIterator orbit(v);
    QuadEdge::Edge *e;
    while ((e = orbit.next()) != 0) {
        if (e->Right() == right) {
            right_idx = i;
        }
        rotated_orbit.push_back(e);
        ++i;
    }

    // move the left-right range to the beginning of list.
    std::rotate(begin(rotated_orbit),
                std::next(begin(rotated_orbit), right_idx),
                end(rotated_orbit));

    // pop faces and edges in left-right range.
    for (auto edge : rotated_orbit) {
        SigPopFace(edge->Right());
        if (edge->Right() == left) {
            break;
        }
        SigPopEdge(edge);
    }

    // make topological changes to QuadEdge cell, set new vertex position.
    QuadEdge::Edge* enew = terrain_->makeVertexEdge(v, left, right);
    QuadEdge::Vertex* vnew = enew->Dest();
    vnew->pos = vnew_pos;

    // we assume vnew_pos is already registered, so just push the vertex.
    SigPushVertex(vnew);

    // push all new edges and faces.
    QuadEdge::VertexEdgeIterator orbitnew(vnew);
    while ((e = orbitnew.next()) != 0) {
        SigPushEdge(e);
        SigPushFace(e->Left());
    }

    return enew;
}

QuadEdge::Edge* RegionalTerrain_3r::MakeFaceEdge(QuadEdge::Face *f,
                                                 QuadEdge::Vertex *org,
                                                 QuadEdge::Vertex *dest) {
    SigPopFace(f);
    QuadEdge::Edge* e = terrain_->makeFaceEdge(f, org, dest);
    SigPushFace(e->Left());
    SigPushFace(e->Right());
    SigPushEdge(e);
    return e;
}

void RegionalTerrain_3r::KillVertexEdge(QuadEdge::Edge *e) {
    QuadEdge::Face *left = e->Left();
    QuadEdge::Face *right = e->Right();
    QuadEdge::Vertex *v = e->Org();

    QuadEdge::VertexEdgeIterator orbit(e->Dest());
    QuadEdge::Edge *eold;
    while ((eold = orbit.next()) != 0) {
        SigPopEdge(eold);
        SigPopFace(eold->Left());
    }
    SigPopVertex(e->Dest());
    terrain_->killVertexEdge(e);

    // we need to pop all faces and edges on the ccw traveral from the left
    // face to the right face in v's orbit. the vertexedgeiterator will begin
    // at a random edge in the orbit, and we have no way of determining whether
    // we are inside or outside the left-to-right range on the first pass
    // through the edges. the iterator will give us the edges back in ccw order
    // however.

    // the strategy is to place all edges in v's orbit into a list and rotate
    // the list so that the leftmost edge to pop is the first element.
    std::list<QuadEdge::Edge*> rotated_orbit;

    // iterate through the orbit and track the leftmost edge insertion index.
    size_t right_idx = 0;
    size_t i = 0;
    QuadEdge::VertexEdgeIterator orbitnew(v);
    QuadEdge::Edge *enew;
    while ((enew = orbitnew.next()) != 0) {
        if (enew->Right() == right) {
            right_idx = i;
        }
        rotated_orbit.push_back(enew);
        ++i;
    }

    // move the left-right range to the beginning of list.
    std::rotate(begin(rotated_orbit),
                std::next(begin(rotated_orbit), right_idx),
                end(rotated_orbit));

    // push faces and edges in left-right range.
    for (auto edge : rotated_orbit) {
        SigPushFace(edge->Right());
        if (edge->Right() == left) {
            break;
        }
        SigPushEdge(edge);
    }
}

void RegionalTerrain_3r::KillFaceEdge(QuadEdge::Edge *e) {
    QuadEdge::Face* left = e->Left();
    QuadEdge::Face* right = e->Right();
    SigPopFace(left);
    SigPopFace(right);
    SigPopEdge(e);
    terrain_->killFaceEdge(e);
    SigPushFace(left);
}

size_t RegionalTerrain_3r::EdgeCount(QuadEdge::Face *f) {
    size_t count = 0;

    QuadEdge::FaceEdgeIterator edges(f);
    QuadEdge::Edge *e;
    while ((e = edges.next()) != 0) {
        ++count;
    }

    return count;
}

} // namespace DDAD
