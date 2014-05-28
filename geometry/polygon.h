/*
 * This file is part of RationalCAD.
 *
 * RationalCAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * RationalCAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with RationalCAD. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @brief 2-dimensional polygonal chain and polygon types.
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 02/12/2013
 */

#ifndef GE_POLYGON_H
#define GE_POLYGON_H

#include "common.h"
#include "visual.h"
#include "point.h"
#include "line.h"
#include "triangle.h"

namespace DDAD {

//=============================================================================
// Interface: PolyChainVertex_2r
//=============================================================================

class PolyChainVertex_2r {
public:
    PolyChainVertex_2r();
    explicit PolyChainVertex_2r(const Point_2r& vertex);
    explicit PolyChainVertex_2r(SharedPoint_2r vertex);

    const Point_2r& vertex() const;
    const Segment_2r& edge_prev() const;
    const Segment_2r& edge_next() const;
    SharedPoint_2r vertex_sptr();

    void set_vertex(const Point_2r& v);
    void set_edge_prev(const Segment_2r& e);
    void set_edge_next(const Segment_2r& e);
    void set_vertex_sptr(SharedPoint_2r v);

private:
    SharedPoint_2r vertex_;
    Segment_2r edge_prev_;
    Segment_2r edge_next_;
};

namespace Predicate {
    bool AIsLeftOfB(const PolyChainVertex_2r& a, const PolyChainVertex_2r& b);
    bool AIsBelowB(const PolyChainVertex_2r& a, const PolyChainVertex_2r& b);
}

//=============================================================================
// Interface: PolyChain_2r
//=============================================================================

class PolyChain_2r : public Visual::Geometry {
public:
    PolyChain_2r();
    ~PolyChain_2r();

    void AppendVertex(const Point_2r& v);
    void AppendVertex(SharedPoint_2r v);
    void RemoveFront();
    void Close();
    void RotateToMaxX();
    PolyChainVertex_2r& back();

    const std::list<PolyChainVertex_2r>& vertices() const;
    const bool closed() const;
    void set_vertices(const std::list<PolyChainVertex_2r>& vertices);
    void set_closed(const bool closed);

    typedef std::list<PolyChainVertex_2r>::const_iterator const_iterator;

    friend class Polygon_2r;

private:
    std::list<PolyChainVertex_2r> vertices_;
    bool closed_;
};

//=============================================================================
// Interface: Polygon_2r
//=============================================================================

class Polygon_2r : public Visual::Geometry {
public:
    Polygon_2r();
    ~Polygon_2r();

    void AppendVertexToBoundary(const Point_2r& v);
    void AppendVertexToBoundary(SharedPoint_2r v);
    void CloseBoundary();
    void ComputeIntegerHull();
    const size_t NumVertices() const;

    const PolyChain_2r& boundary() const;

    const Visual::Color& diffuse() const { return diffuse_; }
    void set_diffuse(const Visual::Color& diffuse) { diffuse_ = diffuse; }

private:
    PolyChain_2r boundary_;
    std::vector<Triangle_2r> triangulation_;
    Visual::Color diffuse_;
};

Polygon_2r Melkman(const PolyChain_2r& P,
                   Visual::IGeometryObserver* observer = nullptr);

//Polygon_2r IntegerHull(const Polygon_2r& P, IGeometryObserver* observer = nullptr);

} // namespace DDAD

#endif // GE_POLYGON_H
