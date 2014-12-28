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
#include "arithmetic.h"
#include "matrix.h"
#include "polytope.h"
#include "intersection.h"
#include "quadedge.h"

namespace DDAD {

//=============================================================================
// Polytope_3r
//=============================================================================

Polytope_3r::Polytope_3r() {}

Polytope_3r::~Polytope_3r() {
    LOG(DEBUG) << "destroying polytope...";

    // remove faces and edges
    QuadEdge::CellFaceIterator cellFaces(cell_);
    QuadEdge::Face *f;
    while ((f = cellFaces.next()) != 0) {
        QuadEdge::FaceEdgeIterator faceEdges(f);
        QuadEdge::Edge *e;

        e = faceEdges.next();
        if (e->Org() < e->Dest()) {
            SigPopVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos));
        }
        auto fan_pivot = e->Org()->pos;
        e = faceEdges.next();
        if (e->Org() < e->Dest()) {
            SigPopVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos));
        }
        auto fan_middle = e->Org()->pos;
        e = faceEdges.next();
        if (e->Org() < e->Dest()) {
            SigPopVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos));
        }
        auto fan_last = e->Org()->pos;
        Triangle_3r tri0(fan_pivot, fan_middle, fan_last);
        SigPopVisualTriangle_3r(tri0);
        while ((e = faceEdges.next()) != 0) {
            fan_middle = fan_last;
            fan_last = e->Org()->pos;
            Triangle_3r tri(fan_pivot, fan_middle, fan_last);
            SigPopVisualTriangle_3r(tri);
            if (e->Org() < e->Dest()) {
                SigPopVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos));
            }
        }
    }

    // remove vertices
    QuadEdge::CellVertexIterator cellVerts(cell_);
    QuadEdge::Vertex *v;
    while ((v = cellVerts.next()) != 0) {
        SigPopVisualPoint_3r(*v->pos);
    }
}

void Polytope_3r::Initialize(const Point_3f& start, const Point_3f& cur) {
    cell_ = QuadEdge::Cell::make();
    start_ = start;

    // grab the initial vertex
    QuadEdge::Vertex *vertex1; {
        QuadEdge::CellVertexIterator iterator(cell_);
        vertex1 = iterator.next();
        assert(vertex1 != 0);
    }

    // grab the initial edge and the initial faces
    QuadEdge::Edge *edge1 = vertex1->getEdge();
    QuadEdge::Face *left  = edge1->Left();
    QuadEdge::Face *right = edge1->Right();

    QuadEdge::Vertex *vertex2 = cell_->makeVertexEdge(vertex1, left, right)->Dest();
    QuadEdge::Vertex *vertex3 = cell_->makeVertexEdge(vertex2, left, right)->Dest();
    QuadEdge::Vertex *vertex4 = cell_->makeVertexEdge(vertex3, left, right)->Dest();
    QuadEdge::Vertex *vertex5 = cell_->makeVertexEdge(vertex4, left, right)->Dest();
    QuadEdge::Vertex *vertex6 = cell_->makeVertexEdge(vertex5, left, right)->Dest();
    QuadEdge::Vertex *vertex7 = cell_->makeVertexEdge(vertex6, left, right)->Dest();
    QuadEdge::Vertex *vertex8 = cell_->makeVertexEdge(vertex7, left, right)->Dest();

    float minX = std::min(start.x(), cur.x());
    float maxX = std::max(start.x(), cur.x());
    float minY = std::min(start.y(), cur.y());
    float maxY = std::max(start.y(), cur.y());
    float minZ = std::min(start.z(), cur.z());
    float maxZ = std::max(start.z(), cur.z());

    vertex1->pos = std::make_shared<Point_3r>(maxX, minY, minZ);
    vertex2->pos = std::make_shared<Point_3r>(minX, minY, minZ);
    vertex3->pos = std::make_shared<Point_3r>(minX, minY, maxZ);
    vertex4->pos = std::make_shared<Point_3r>(minX, maxY, maxZ);
    vertex5->pos = std::make_shared<Point_3r>(minX, maxY, minZ);
    vertex6->pos = std::make_shared<Point_3r>(maxX, maxY, minZ);
    vertex7->pos = std::make_shared<Point_3r>(maxX, maxY, maxZ);
    vertex8->pos = std::make_shared<Point_3r>(maxX, minY, maxZ);

    // construct the bottom face
    auto bfE = cell_->makeFaceEdge(left, vertex6, vertex1);
    QuadEdge::Face *bottom = bfE->Left();
    auto rfE = cell_->makeFaceEdge(bottom, vertex5, vertex2);
    auto fE = cell_->makeFaceEdge(right, vertex7, vertex4);
    auto tE = cell_->makeFaceEdge(fE->Left(), vertex8, vertex3);

    // register vertices
    Visual::Material vMat;
    vMat.set_ambient(Visual::Color::BLACK);
    Visual::Point vPoint(vMat);

    QuadEdge::CellVertexIterator cellVerts(cell_);
    QuadEdge::Vertex *v;
    while ((v = cellVerts.next()) != 0) {
        SigRegisterPoint_3r(*v->pos);
        SigPushVisualPoint_3r(*v->pos, vPoint);
    }

    /* Stepping through the (undirected) edges of a cell is more complex, as
     * we have things set up. Note that there are twice as many directed edges
     * as undirected edges. The above code visits all directed edges once, so
     * it visits each undirected edge twice. But for wireframe drawing and many
     * other purposes you would want to operate on each undirected edge just
     * once. A clever way to guarantee this, without marking edges or any
     * additional arrays or lists, is to visit each undirected edge twice, but
     * use the fact that the pointers to the two vertices are addresses, one of
     * which is larger than the other:

    CellFaceIterator cellFaces(c);
    Face *f;
    while ((f = cellFaces.next()) != 0) {
        // visit each face of cell c
        FaceEdgeIterator faceEdges(f);
        Edge *edge;
        while ((edge = faceEdges.next()) != 0) {
            // visit each edge of face f
            // if edge passes the following, its Sym will not,
            // and vice-versa
            if (edge->Org() < edge->Dest())
                <do something with edge>
        }
    }

    */

    // register faces
    Visual::Material eMat;
    eMat.set_ambient(Visual::Color::BLACK);
    Visual::Segment vSegment(eMat);

    Visual::Material fMat;
    fMat.set_ambient(Visual::Color::GRAY);
    Visual::Triangle vTriangle(fMat);

    QuadEdge::CellFaceIterator cellFaces(cell_);
    QuadEdge::Face *f;
    while ((f = cellFaces.next()) != 0) {
        QuadEdge::FaceEdgeIterator faceEdges(f);
        QuadEdge::Edge *e;

        e = faceEdges.next();
        if (e->Org() < e->Dest()) {
            SigPushVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos),
                                    vSegment);
        }
        auto fan_pivot = e->Org()->pos;
        e = faceEdges.next();
        if (e->Org() < e->Dest()) {
            SigPushVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos),
                                    vSegment);
        }
        auto fan_middle = e->Org()->pos;
        e = faceEdges.next();
        if (e->Org() < e->Dest()) {
            SigPushVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos),
                                    vSegment);
        }
        auto fan_last = e->Org()->pos;
        Triangle_3r tri0(fan_pivot, fan_middle, fan_last);
        SigPushVisualTriangle_3r(tri0, vTriangle);
        while ((e = faceEdges.next()) != 0) {
            fan_middle = fan_last;
            fan_last = e->Org()->pos;
            Triangle_3r tri(fan_pivot, fan_middle, fan_last);
            SigPushVisualTriangle_3r(tri, vTriangle);
            if (e->Org() < e->Dest()) {
                SigPushVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos),
                                        vSegment);
            }
        }
    }
}

void Polytope_3r::Update(const Point_3f& cur) {
    if (cur == start_) {
        return;
    }

    float minX = std::min(start_.x(), cur.x());
    float maxX = std::max(start_.x(), cur.x());
    float minY = std::min(start_.y(), cur.y());
    float maxY = std::max(start_.y(), cur.y());
    float minZ = std::min(start_.z(), cur.z());
    float maxZ = std::max(start_.z(), cur.z());

    std::array<rational, 3> v1 = { maxX, minY, minZ };
    std::array<rational, 3> v2 = { minX, minY, minZ };
    std::array<rational, 3> v3 = { minX, minY, maxZ };
    std::array<rational, 3> v4 = { minX, maxY, maxZ };
    std::array<rational, 3> v5 = { minX, maxY, minZ };
    std::array<rational, 3> v6 = { maxX, maxY, minZ };
    std::array<rational, 3> v7 = { maxX, maxY, maxZ };
    std::array<rational, 3> v8 = { maxX, minY, maxZ };

    QuadEdge::CellVertexIterator cellVerts(cell_);
    QuadEdge::Vertex *v;

    v = cellVerts.next();
    v->pos->set_elements(v1);
    v->pos->SigPositionChanged();
    v = cellVerts.next();
    v->pos->set_elements(v2);
    v->pos->SigPositionChanged();
    v = cellVerts.next();
    v->pos->set_elements(v3);
    v->pos->SigPositionChanged();
    v = cellVerts.next();
    v->pos->set_elements(v4);
    v->pos->SigPositionChanged();
    v = cellVerts.next();
    v->pos->set_elements(v5);
    v->pos->SigPositionChanged();
    v = cellVerts.next();
    v->pos->set_elements(v6);
    v->pos->SigPositionChanged();
    v = cellVerts.next();
    v->pos->set_elements(v7);
    v->pos->SigPositionChanged();
    v = cellVerts.next();
    v->pos->set_elements(v8);
    v->pos->SigPositionChanged();

    SigUpdate();
}

/*
 * GOAL:
 *
 * Compute the integer hull of a polytope with rational vertex coordinates.
 *
 *
 *
 * Our main algorithm computes a Dobkin-Kirkpatrick hierarchy. At each round
 * of the DK computation, we choose a maximal independent set of vertices from
 * the polytope to remove. The idea is to consider each of these individually
 * and compute their integer hull in isolation.
 *
 * SUBGOAL:
 *
 * Compute the integer hull of a tetrahedron.
 *
 *
 * SIMPLEST QUESTION:
 *
 * Compute the integer hull of grid points behind an arbitrary plane.
 *
 * This question makes sense, but the answer may not be very interesting.
 * Since this plane is arbitrary, we have two cases:
 *
 * case 1: the plane passes through infinitely many grid points.
 * case 2: the plane passes through no grid points.
 *
 * In the first case, the integer hull is - imprecisely speaking - the
 * halfspace defined by the plane equation.
 *
 * In the second case, the integer hull is - imprecisely speaking - the
 * halfspace defined by shifting the plane inward along its normal until it
 * hits the first grid point. Once it hits one grid point, we have hit an
 * infinite plane of grid points, so we are back to the first case.
 *
 * NEXT QUESTION:
 *
 * Compute the integer hull of grid points behind two arbitrary planes.
 *
 * Given two arbitrary planes, we have a large number of situations that may
 * arise. Taken individually, each plane may be either of the two cases
 * outlined above. Taken together, they can:
 *
 * case 1: intersect in a line
 * case 2: have no intersection (parallel)
 * case 3: intersect in a plane (coplanar)
 *
 * Let's deal with the degenerate cases first. In case 2, the planes are
 * parallel. If they share the same normal, then one of the planes is redundant
 * and we are reduced to the previous question. If they have normals of opposite
 * signs, then the intersection of their halfspaces will be either the empty set
 * or an infinite slab of R^3. The empty case is easy to detect by simply
 * ordering the planes along their common axis. The slab case is more interesting.
 *
 * SLAB
 *
 * In the slab case, we can have 4 cases corresponding to the 4 different combinations
 * of the individual planes intersecting grid points or not. If neither of them
 * contain grid points initially, then we independently shift the planes inward
 * along their respective normals. This can result in the planes intersecting the same
 * set of grid points, or different sets of grid points. If the sets are equal,
 * then the integer hull is an infinitely thin disk parallel with the planes.
 * If they are not equal, then the integer hull is an infinite slab parallel with
 * the planes.
 *
 * COPLANAR
 *
 * If the planes are originally coplanar, then they may again have normals with
 * the same sign or opposite signs. If they have the same sign, then we are
 * reduced to the previous question since the planes are exactly equal. If they
 * have opposite signs, then the integer hull is either the infinitely thin
 * disk from the previous analysis (if the planes pass through grid points) or
 * the integer hull is the empty set.
 *
 * LINE
 *
 * Finally, the planes may intersect in a line. This is the first time where
 * the integer hull can actually be complicated. We have a large number of cases
 * to explore. We can note the following:
 *
 * - Either plane may again intersect grid points or not
 * - If either plane does not intersect any grid points, then the line of
 *   intersection also doesn't have any grid points (since its a subspace of
 *   that plane)
 * - If the line of intersection passes through grid points, then both planes
 *   also contain grid points. In this case, the integer hull is the infinite
 *   volume of R^3 defined by the two planes.
 * - We should be able to have cases where both planes pass through grid points
 *   individually, but the line of intersection does not contain grid points.
 * - If both planes pass through grid points, but the line of intersection
 *   does not, then the integer hull is described as follows. Compute the line
 *   of intersection. Consider this line with respect to one of the planes.
 *   In that plane, compute the normal to the line such that the normal points
 *   into the halfspace defined by the other plane. Shift a copy of the line
 *   inward along this normal until the first infinite row of grid points is
 *   hit. Repeat this procedure with the original line and the other plane to
 *   produce another line in the other plane. The integer hull is then defined
 *   by the convex hull of the original planes up to these cutoff lines?
 *
 *
 * - Choose a grid point on one of the planes to serve as the origin. Flatten
 *   that plane the normal way, using the line of intersection as the first
 *   vector, and apply the same transformation to the second plane. What does
 *   the second plane look like?
 *
 * - There should be one component of the second plane aligned with one of the
 *   axes in the final space. The other two components are unknown, and the
 *   plane is not passing through the origin. However, since the plane originally
 *   passed through integer points, and the transformation was unimodular, the
 *   second plane should still be passing through grid points (just not the origin).
 *
 *
 * One way of making the problem more complicated is to ask
 *
 * It does not make sense to ask what the integer hull below plane is.
 *
 *
 * I want to see which parts of the integer hull are identified by the process
 * of flattening out a plane (if any).
 *
 * One way of investigating this is as follows: Create a simplicial cone with
 * its vertex at the origin. Brute-force compute the integer hull. This is
 * problematic because the cone is not bounded.
 *
 * Another way could instead create a tetrahedron with one vertex at the
 * origin. This would be easier to brute force compute the ihull because the
 * volume can be bounded by an AABB. This would require implementing a 3D
 * convex hull algorithm to feed grid points into.
 *
 * 1. Need ability to create tetrahedra with one vertex at the origin.
 *      - this means that we are first investigating
 */

namespace Construction {

/*!
 * Implementation of a randomized convex hull algorithm.
 */
Polytope_3r ConvexHull(const std::vector<SharedPoint_3r>& P) {
    Polytope_3r hull;

    
    // find four points p1, p2, p3, p4 in P that form a tetrahedron

    // C = CH({p1, p2, p3, p4})

    // compute a random permutation p5, p6, ..., pn of the remaining points.

    // initialize the conflict graph G with all visible pairs (pt, f) where f
    // is a facet of C and t > 4.

    // for r = 5 to n do
    //  if F_conflict(pr) is not empty
    //  then delete all facets in F_conflict(pr) from C
    //


    return hull;
}

}



Vector_3i ClearFractions(const Vector_3r& v) {
    return Vector_3i(v.x().get_num()*v.y().get_den()*v.z().get_den(),
                     v.y().get_num()*v.x().get_den()*v.z().get_den(),
                     v.z().get_num()*v.x().get_den()*v.y().get_den());
}

Vector_3i RemoveGCD(const Vector_3i& v) {
    return RemoveFactor(v, GCD(v));
}

Vector_2i RemoveGCD(const Vector_2i& v) {
    return RemoveFactor(v, GCD(v));
}


Polytope_3r IntegerHull(const Polytope_3r& P) {




    return Polytope_3r();
}

/*!
 * Input: A vector v whose components are relatively prime. A 3x3 unimodular
 * matrix B whose rows form a basis for Z^3.
 *
 * Output: Either a unimodular matrix B' with one row equal to v, or the
 * identity matrix if no such matrix can be found.
 *
 * Invariants: Every matrix given to a recursive call is unimodular.
 *
 * Notes: Unimodular means det(B) = +-1. split basis into 6 tetrahedra.
 * rewrite vector in terms of input basis. sort vector coordinates by
 * magnitude. use the ordering to determine which of the 6 tetrahedra the
 * vector lies within, and branch accordingly.
 */
Matrix_3x3i UnimodularBasisForVector_r(const Vector_3i& v,
                                       const Matrix_3x3i& B) {

    // base case: the input basis contains the vector
    if (RowVec(B, 0) == v || RowVec(B, 1) == v || RowVec(B, 2) == v) {
        assert(Determinant(B) == 1 || Determinant(B) == -1);
        assert(GCD(RowVec(B, 0)) == 1);
        assert(GCD(RowVec(B, 1)) == 1);
        assert(GCD(RowVec(B, 2)) == 1);
        return B;
    }

    Vector_3i vPrime = v*Inverse(B);
    Matrix_3x3i BPrime = B;

    // sort vPrime's components and keep track of indices
    std::array<std::pair<integer, size_t>, 3> vPrimeElts;
    vPrimeElts[0].first = vPrime.x(); vPrimeElts[0].second = 0;
    vPrimeElts[1].first = vPrime.y(); vPrimeElts[1].second = 1;
    vPrimeElts[2].first = vPrime.z(); vPrimeElts[2].second = 2;
    std::sort(begin(vPrimeElts), end(vPrimeElts), []
    (const std::pair<integer, size_t>& lhs,
     const std::pair<integer, size_t>& rhs) {
        return lhs.first < rhs.first;
    });

    BPrime(vPrimeElts[0].second, 0) += BPrime(vPrimeElts[2].second, 0);
    BPrime(vPrimeElts[0].second, 1) += BPrime(vPrimeElts[2].second, 1);
    BPrime(vPrimeElts[0].second, 2) += BPrime(vPrimeElts[2].second, 2);

    BPrime(vPrimeElts[0].second, 0) += BPrime(vPrimeElts[1].second, 0);
    BPrime(vPrimeElts[0].second, 1) += BPrime(vPrimeElts[1].second, 1);
    BPrime(vPrimeElts[0].second, 2) += BPrime(vPrimeElts[1].second, 2);

    BPrime(vPrimeElts[1].second, 0) += BPrime(vPrimeElts[2].second, 0);
    BPrime(vPrimeElts[1].second, 1) += BPrime(vPrimeElts[2].second, 1);
    BPrime(vPrimeElts[1].second, 2) += BPrime(vPrimeElts[2].second, 2);

    return UnimodularBasisForVector_r(v, BPrime);
}

/*!
 * See comments for recursive function above.
 */
Matrix_3x3i UnimodularBasisForVector(const Vector_3i& v) {
    // correctly orient starting basis
    Matrix_3x3i signMat(v.x() < 0 ? -1 : 1, 0, 0,
                        0, v.y() < 0 ? -1 : 1, 0,
                        0, 0, v.z() < 0 ? -1 : 1);

    // recursively find a basis starting with the identity
    return UnimodularBasisForVector_r(RemoveGCD(v), signMat);
}

Matrix_2x2i UnimodularBasisForVector_r(const Vector_2i& v,
                                       const Matrix_2x2i& B) {
    // base case: the input basis contains the vector
    if (RowVec(B, 0) == v || RowVec(B, 1) == v) {
        assert(Determinant(B) == 1 || Determinant(B) == -1);
        assert(GCD(RowVec(B, 0)) == 1);
        assert(GCD(RowVec(B, 1)) == 1);
        return B;
    }

    Vector_2i vPrime = v*Inverse(B);
    Matrix_2x2i BPrime = B;

    if (vPrime.x() > vPrime.y()) {
        BPrime(1, 0) += BPrime(0, 0);
        BPrime(1, 1) += BPrime(0, 1);
    } else {
        BPrime(0, 0) += BPrime(1, 0);
        BPrime(0, 1) += BPrime(1, 1);
    }

    return UnimodularBasisForVector_r(v, BPrime);
}

Matrix_2x2i UnimodularBasisForVector(const Vector_2i& v) {
    // correctly orient starting basis
    Matrix_2x2i signMat(v.x() < 0 ? -1 : 1, 0,
                        0, v.y() < 0 ? -1 : 1);

    // recursively find a basis starting with the identity
    return UnimodularBasisForVector_r(RemoveGCD(v), signMat);
}

/*!
 * Iterative version of unimodular basis for vector.
 */
Matrix_3x3i UnimodularBasisForVectorItr(const Vector_3i& v) {
    auto shortV = RemoveGCD(v);

    // correctly orient starting basis
    Matrix_3x3i basis(shortV.x() < 0 ? -1 : 1, 0, 0,
                      0, shortV.y() < 0 ? -1 : 1, 0,
                      0, 0, shortV.z() < 0 ? -1 : 1);

    while (RowVec(basis, 0) != shortV && RowVec(basis, 1) != shortV &&
           RowVec(basis, 2) != shortV) {
        // rewrite v in the newest basis
        Vector_3i vPrime = shortV*Inverse(basis);

        // sort vPrime's components and keep track of indices
        std::array<std::pair<integer, size_t>, 3> vPrimeElts;
        vPrimeElts[0].first = vPrime.x(); vPrimeElts[0].second = 0;
        vPrimeElts[1].first = vPrime.y(); vPrimeElts[1].second = 1;
        vPrimeElts[2].first = vPrime.z(); vPrimeElts[2].second = 2;
        std::sort(begin(vPrimeElts), end(vPrimeElts), []
        (const std::pair<integer, size_t>& lhs,
         const std::pair<integer, size_t>& rhs) {
            return lhs.first < rhs.first;
        });

//        basis(vPrimeElts[0].second, 0) += basis(vPrimeElts[2].second, 0);
//        basis(vPrimeElts[0].second, 1) += basis(vPrimeElts[2].second, 1);
//        basis(vPrimeElts[0].second, 2) += basis(vPrimeElts[2].second, 2);

//        basis(vPrimeElts[0].second, 0) += basis(vPrimeElts[1].second, 0);
//        basis(vPrimeElts[0].second, 1) += basis(vPrimeElts[1].second, 1);
//        basis(vPrimeElts[0].second, 2) += basis(vPrimeElts[1].second, 2);

        basis(vPrimeElts[1].second, 0) += basis(vPrimeElts[2].second, 0);
        basis(vPrimeElts[1].second, 1) += basis(vPrimeElts[2].second, 1);
        basis(vPrimeElts[1].second, 2) += basis(vPrimeElts[2].second, 2);
    }

    // permute the rows such that v is always the first
    /*
    int row;
    if (RowVec(basis, 0) == shortV) {
        row = 0;
    } else if (RowVec(basis, 1) == shortV) {
        row = 1;
    } else {
        row = 2;
    }
    */

    // verify output assumptions
    assert(Determinant(basis) == 1 || Determinant(basis) == -1);
    assert(GCD(RowVec(basis, 0)) == 1);
    assert(GCD(RowVec(basis, 1)) == 1);
    assert(GCD(RowVec(basis, 2)) == 1);

    // two rows must be bounded by input length

    return basis;
}

Matrix_2x2i UnimodularBasisForVectorItr(const Vector_2i& v) {
    auto shortV = RemoveGCD(v);

    // correctly orient starting basis
    Matrix_2x2i basis(shortV.x() < 0 ? -1 : 1, 0,
                      0, shortV.y() < 0 ? -1 : 1);

    while (RowVec(basis, 0) != shortV && RowVec(basis, 1) != shortV) {
        Vector_2i vPrime = shortV*Inverse(basis);

        if (vPrime.x() > vPrime.y()) {
            basis(1, 0) += basis(0, 0);
            basis(1, 1) += basis(0, 1);
        } else {
            basis(0, 0) += basis(1, 0);
            basis(0, 1) += basis(1, 1);
        }
    }

    // verify output assumptions
    assert(Determinant(basis) == 1 || Determinant(basis) == -1);
    assert(GCD(RowVec(basis, 0)) == 1);
    assert(GCD(RowVec(basis, 1)) == 1);

    return basis;
}

Matrix_3x3i UnimodularBasisForPlane(const Vector_3i& u, const Vector_3i& v) {
    std::cout << "Starting UnimodularBasisForPlane with u = "
              << u << " and v = " << v << std::endl;

    // bring u into a unimodular basis
    auto basisU = UnimodularBasisForVectorItr(u);
    std::cout << "UnimodularBasisForVectorItr produced basisU = \n"
              << basisU << std::endl;

    // rewrite v in terms of u's basis
    auto vPrime = v*Inverse(basisU);
    std::cout << "vPrime " << vPrime << std::endl;

    // bring v into a unimodular basis
    auto basisV = UnimodularBasisForVectorItr(Vector_2i(vPrime.y(),
                                                        vPrime.z()));
    std::cout << "UnimodularBasisForVectorItr produced basisV = \n"
              << basisV << std::endl;

    return Matrix_3x3i(1, 0, 0,
                       0, basisV(0,0), basisV(0,1),
                       0, basisV(1,0), basisV(1,1))*basisU;
}



namespace Construction {

Polytope_3r IntegerHull(const Polytope_3r& P) {
    Polytope_3r IHP;

    return IHP;
}

}

/*!
 * INPUT. 2 affine planes, P and Q, that intersect in a line L.
 *
 * OUTPUT. The integer hull of the region behind or on both P and Q, where
 * the hull is represented as a collection of planes.
 *
 * ALGORITHM. Construct an integer vector that points along the direction of
 * L (it should not matter which orientation). Construct a second integer
 * vector that together with the first vector spans P. Use these vectors to
 * construct a basis such that P is now a coordinate plane in two of the
 * dimensions, and transform Q into this new space (unimodular).
 *
 * In this new space, the line of intersection L is parallel to one of the
 * coordinate axes, and is contained within the coordinate plane corresponding
 * to P. The second plane Q is at some rotation around L. We care about the area
 * above the coordinate plane and below transformed Q. The integer hull of this
 * area, transformed back into the original space, will give the integer hull
 * of the original region.
 */
void ihull() {

    // create plane P from three rational points
    auto planeptP0 = std::make_shared<Point_3r>(1, 2, 0);
    auto planeptP1 = std::make_shared<Point_3r>(5, 1, 2);
    auto planeptP2 = std::make_shared<Point_3r>(1, 16, 7);
    std::cout << "planeptP0 " << *planeptP0 << std::endl;
    std::cout << "planeptP1 " << *planeptP1 << std::endl;
    std::cout << "planeptP2 " << *planeptP2 << std::endl;

    // compute P's normal
    auto planevecP0 = *planeptP1-*planeptP0;
    auto planevecP1 = *planeptP2-*planeptP0;
    auto planenormP = Cross(planevecP0, planevecP1);
    std::cout << "planenormP " << planenormP << std::endl;

    // create plane Q from three rational points
    auto planeptQ0 = std::make_shared<Point_3r>(2, 1, 0);
    auto planeptQ1 = std::make_shared<Point_3r>(16, 6, 5);
    auto planeptQ2 = std::make_shared<Point_3r>(10, 19, 6);
    std::cout << "planeptQ0 " << *planeptQ0 << std::endl;
    std::cout << "planeptQ1 " << *planeptQ1 << std::endl;
    std::cout << "planeptQ2 " << *planeptQ2 << std::endl;

    // compute Q's normal
    auto planevecQ0 = *planeptQ1-*planeptQ0;
    auto planevecQ1 = *planeptQ2-*planeptQ0;
    auto planenormQ = Cross(planevecQ0, planevecQ1);
    std::cout << "planenormQ " << planenormQ << std::endl;

    // compute a vector along the line of intersection L between P and Q
    // since L lies in both planes, it must be perpendicular to both normals
    auto linevecL = RemoveGCD(ClearFractions(Cross(planenormP, planenormQ)));
    std::cout << "linevecL " << linevecL << std::endl;

    // choose a second vector that, with L's direction vector, spans P.
    // for now, assume planevecP0 != linevecL
    auto spanvecP0 = RemoveGCD(ClearFractions(planevecP0));
    assert(spanvecP0 != linevecL);
    std::cout << "spanvecP0 " << spanvecP0 << std::endl;

    // compute a basis in which L is the x axis and P is parallel to the
    // xy coordinate plane
    auto basisFinal = UnimodularBasisForPlane(linevecL, spanvecP0);
    std::cout << "UnimodularBasisForPlane produced \n" << basisFinal << std::endl;

    auto inverseBasis = Inverse(basisFinal);

    // rewrite P's points
    auto planeptP0Prime = std::make_shared<Point_3r>(*planeptP0*inverseBasis);
    auto planeptP1Prime = std::make_shared<Point_3r>(*planeptP1*inverseBasis);
    auto planeptP2Prime = std::make_shared<Point_3r>(*planeptP2*inverseBasis);

    std::cout << "planeptP0Prime: " << *planeptP0Prime << std::endl;
    std::cout << "planeptP1Prime: " << *planeptP1Prime << std::endl;
    std::cout << "planeptP2Prime: " << *planeptP2Prime << std::endl;

    // the z coordinate of all of P's points should be equal, but in general
    // will not be 0
    //assert(planeptP0Prime->z() == planeptP1Prime->z() == planeptP2Prime->z());

    // the z coordinate defines a line in the yz plane in the new space
    // we are concerned with grid points above this line
    // todo : compute line

    // rewrite Q's points
    auto planeptQ0Prime = std::make_shared<Point_3r>(*planeptQ0*inverseBasis);
    auto planeptQ1Prime = std::make_shared<Point_3r>(*planeptQ1*inverseBasis);
    auto planeptQ2Prime = std::make_shared<Point_3r>(*planeptQ2*inverseBasis);

    std::cout << "planeptQ0Prime: " << *planeptQ0Prime << std::endl;
    std::cout << "planeptQ1Prime: " << *planeptQ1Prime << std::endl;
    std::cout << "planeptQ2Prime: " << *planeptQ2Prime << std::endl;

    // the projection of Q onto the yz plane should produce a second line
    // we are concerned with grid points below this line
    // todo: compute line

    // given the 2 halfspaces in the yz plane, compute the integer hull using
    // the 2D algorithm

    // vertices identified by the hull algorithm

    /*


    // compute a unimodular basis with L's direction vector
    auto basisL = UnimodularBasisForVectorItr(linevecL);
    std::cout << "basisL\n" << basisL << std::endl;







    // rewrite the second vector in the new basis
    auto spanvecP0Prime = RemoveGCD(spanvecP0*Inverse(basisL));
    std::cout << "spanvecP0Prime " << spanvecP0Prime << std::endl;

    // compute
    Matrix_2x2i basis2P0;
    Matrix_3x3i basis3P0;
    if (RowVec(basisL, 0) == linevecL) {
        std::cout << "row0" << std::endl;
        basis2P0 = UnimodularBasisForVectorItr(Vector_2i(spanvecP0Prime.y(),
                                                         spanvecP0Prime.z()));
        basis3P0 = Matrix_3x3i(1, 0, 0,
                               0, basis2P0(0,0), basis2P0(0,1),
                               0, basis2P0(1,0), basis2P0(1,1));
        assert(Determinant(basis3P0) == 1 || Determinant(basis3P0) == -1);

    } else if (RowVec(basisL, 1) == linevecL) {
        std::cout << "row1" << std::endl;
        basis2P0 = UnimodularBasisForVectorItr(Vector_2i(spanvecP0Prime.x(),
                                                         spanvecP0Prime.z()));
        basis3P0 = Matrix_3x3i(0, basis2P0(0,0), basis2P0(0,1),
                               1, 0, 0,
                               0, basis2P0(1,0), basis2P0(1,1));
        assert(Determinant(basis3P0) == 1 || Determinant(basis3P0) == -1);
    } else {
        std::cout << "row2" << std::endl;
        basis2P0 = UnimodularBasisForVectorItr(Vector_2i(spanvecP0Prime.x(),
                                                         spanvecP0Prime.y()));
        basis3P0 = Matrix_3x3i(0, basis2P0(0,0), basis2P0(0,1),
                               0, basis2P0(1,0), basis2P0(1,1),
                               1, 0, 0);
        assert(Determinant(basis3P0) == 1 || Determinant(basis3P0) == -1);
    }
    std::cout << "basis3P0\n" << basis3P0 << std::endl;

    auto inverseBasis = Inverse(basisL)*Inverse(basis3P0);
    std::cout << "inverseBasis\n" << inverseBasis << std::endl;
    std::cout << Inverse(basis3P0*basisL) << std::endl;

    // flatten plane P
    auto planeptP0Prime = std::make_shared<Point_3r>(*planeptP0*inverseBasis);
    auto planeptP1Prime = std::make_shared<Point_3r>(*planeptP1*inverseBasis);
    auto planeptP2Prime = std::make_shared<Point_3r>(*planeptP2*inverseBasis);

    // transform plane Q
    auto planeptQ0Prime = std::make_shared<Point_3r>(*planeptQ0*inverseBasis);
    auto planeptQ1Prime = std::make_shared<Point_3r>(*planeptQ1*inverseBasis);
    auto planeptQ2Prime = std::make_shared<Point_3r>(*planeptQ2*inverseBasis);

*/

    /*
    // visualize the plane
    // N.X = 0
    // nx*x+ny*y+nz*z = 0
    // nz*z = -nx*x-ny*y
    // z = (-nx*x-ny*y)/nz
    auto pVec0 = *tb-*ta;
    auto pVec1 = *tc-*ta;
    auto pNorm = Cross(pVec0, pVec1);
    auto ZForXY = [&](const rational& x, const rational& y) -> rational {
        return (-pNorm.x()*x-pNorm.y()*y)/pNorm.z();
    };

    SharedPoint_3r pPt0 = std::make_shared<Point_3r>(-128, -128, ZForXY(-128, -128));
    SharedPoint_3r pPt1 = std::make_shared<Point_3r>( 128, -128, ZForXY( 128, -128));
    SharedPoint_3r pPt2 = std::make_shared<Point_3r>( 128,  128, ZForXY( 128,  128));
    SharedPoint_3r pPt3 = std::make_shared<Point_3r>(-128,  128, ZForXY(-128,  128));

    SigRegisterPoint_3r(*pPt0);
    SigRegisterPoint_3r(*pPt1);
    SigRegisterPoint_3r(*pPt2);
    SigRegisterPoint_3r(*pPt3);
    Triangle_3r pTri0(pPt0, pPt1, pPt2);
    Triangle_3r pTri1(pPt0, pPt2, pPt3);

    Visual::Triangle pVt;
    pVt.set_diffuse(Visual::Color(50, 200, 200, 50));
    */

    /*
    SigPushVisualTriangle_3r(pTri0, pVt);
    SigPushVisualTriangle_3r(pTri1, pVt);
    */


    // visualize triangle
    /*
    vertices_.push_back(ta);
    vertices_.push_back(tb);
    vertices_.push_back(tc);
    SigRegisterPoint_3r(*ta);
    SigRegisterPoint_3r(*tb);
    SigRegisterPoint_3r(*tc);
    SigPushVisualPoint_3r(*ta, Visual::Point());
    SigPushVisualPoint_3r(*tb, Visual::Point());
    SigPushVisualPoint_3r(*tc, Visual::Point());
    Triangle_3r tri(ta, tb, tc);

    std::cout << "Original Triangle:\n" << tri << std::endl;
    Visual::Triangle vt;
    vt.set_diffuse(Visual::Color(50, 200, 200, 255));
    SigPushVisualTriangle_3r(tri, vt);

    // construct two linearly independent integer vectors that span the
    // triangle's plane.

    Vector_3i planeV0 = RemoveGCD(ClearFractions(*tb-*ta));
    Vector_3i planeV1 = RemoveGCD(ClearFractions(*tc-*ta));

    std::cout << "planeV0 = " << planeV0 << std::endl;
    std::cout << "planeV1 = " << planeV1 << std::endl;

    // first we consider just one vector, it can be either but we choose V0.

    auto V0Basis = UnimodularBasisForVector(planeV0);
    std::cout << "V0Basis\n" << V0Basis << std::endl;

    auto invV0Basis = Inverse(V0Basis);
    auto planeV0prime = planeV0*invV0Basis;
    auto planeV1prime = planeV1*invV0Basis;

    std::cout << "planeV0*invV0Basis = " << planeV0prime << std::endl;
    std::cout << "planeV1*invV0Basis = " << planeV1prime << std::endl;

    Matrix_2x2i V1Basis2;
    if (planeV0prime.x() != 0) {
        V1Basis2 = UnimodularBasisForVector(Vector_2i(planeV1prime.y(),
                                                      planeV1prime.z()));
    } else if (planeV0prime.y() != 0) {
        V1Basis2 = UnimodularBasisForVector(Vector_2i(planeV1prime.x(),
                                                      planeV1prime.z()));
    } else {
        V1Basis2 = UnimodularBasisForVector(Vector_2i(planeV1prime.x(),
                                                      planeV1prime.y()));
    }

    Matrix_3x3i V1Basis(1, 0, 0,
                        0, V1Basis2(0, 0), V1Basis2(0, 1),
                        0, V1Basis2(1, 0), V1Basis2(1, 1));

    auto finalBasis = Inverse(V0Basis)*Inverse(V1Basis);

    std::cout << "planeV0*finalBasis = " << planeV0*finalBasis << std::endl;
    std::cout << "planeV1*finalBasis = " << planeV1*finalBasis << std::endl;

    auto fPointA = *ta*finalBasis;

    SharedPoint_3r pivotTriA = std::make_shared<Point_3r>(*ta*finalBasis);
    SharedPoint_3r pivotTriB = std::make_shared<Point_3r>(*tb*finalBasis);
    SharedPoint_3r pivotTriC = std::make_shared<Point_3r>(*tc*finalBasis);

    std::cout << "Pivot Tri Point A: " << *pivotTriA << std::endl;
    std::cout << "Pivot Tri Point B: " << *pivotTriB << std::endl;
    std::cout << "Pivot Tri Point C: " << *pivotTriC << std::endl;

    // visualize triangle

    vertices_.push_back(pivotTriA);
    vertices_.push_back(pivotTriB);
    vertices_.push_back(pivotTriC);
    SigRegisterPoint_3r(*pivotTriA);
    SigRegisterPoint_3r(*pivotTriB);
    SigRegisterPoint_3r(*pivotTriC);
    SigPushVisualPoint_3r(*pivotTriA, Visual::Point());
    SigPushVisualPoint_3r(*pivotTriB, Visual::Point());
    SigPushVisualPoint_3r(*pivotTriC, Visual::Point());
    Triangle_3r pivotTri(pivotTriA, pivotTriB, pivotTriC);
    Visual::Triangle visualPivotTri;
    visualPivotTri.set_diffuse(Visual::Color(255, 255, 0, 255));
    SigPushVisualTriangle_3r(pivotTri, visualPivotTri);
    */

    // draw the coordinate axes

    SharedPoint_3r axesO = std::make_shared<Point_3r>(0, 0, 0);
    SharedPoint_3r axesX = std::make_shared<Point_3r>(32, 0, 0);
    SharedPoint_3r axesY = std::make_shared<Point_3r>(0, 32, 0);
    SharedPoint_3r axesZ = std::make_shared<Point_3r>(0, 0, 32);

/*
    vertices_.push_back(axesO);
    vertices_.push_back(axesX);
    vertices_.push_back(axesY);
    vertices_.push_back(axesZ);
    SigRegisterPoint_3r(*axesO);
    SigRegisterPoint_3r(*axesX);
    SigRegisterPoint_3r(*axesY);
    SigRegisterPoint_3r(*axesZ);
    Segment_3r OX(axesO, axesX);
    Segment_3r OY(axesO, axesY);
    Segment_3r OZ(axesO, axesZ);
    SigPushVisualSegment_3r(OX, Visual::Segment(Visual::Color(255, 0, 0, 255)));
    SigPushVisualSegment_3r(OY, Visual::Segment(Visual::Color(0, 255, 0, 255)));
    SigPushVisualSegment_3r(OZ, Visual::Segment(Visual::Color(0, 0, 255, 255)));
*/
}

} // namespace DDAD
