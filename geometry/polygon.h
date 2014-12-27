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

/*!
 * @brief 2-dimensional polyline and polygon types.
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
// Interface: Polyline_2r
//=============================================================================

class Polyline_2r : public Visual::Geometry {
public:
    Polyline_2r();
    Polyline_2r(const Polyline_2r& copy);
    ~Polyline_2r();

    void Close();
    void Open();
    void RotateToMaxX();

    void push_back(const Point_2r& v);
    void push_back(SharedPoint_2r v);
    void pop_back();
    SharedPoint_2r back(const size_t i = 0) const;

    void push_front(const Point_2r& v);
    void push_front(SharedPoint_2r v);
    void pop_front();
    SharedPoint_2r front(const size_t i = 0) const;

    SharedPoint_2r operator[](const size_t i) const;
    const size_t size() const;

    const std::deque<SharedPoint_2r>& vertices() const;
    const int32_t z_order() const { return z_order_; }
    const Visual::Material& mat_vertex() const { return mat_vertex_; }
    const Visual::Material& mat_edge() const { return mat_vertex_; }
    void set_vertices(const std::deque<SharedPoint_2r>& vertices);
    const bool closed() const;
    void set_mat_vertex(const Visual::Material& mat_vertex) { mat_vertex_ = mat_vertex; }
    void set_mat_edge(const Visual::Material& mat_edge) { mat_edge_ = mat_edge; }
    void set_z_order(const int32_t z_order) { z_order_ = z_order; }

private:
    std::deque<SharedPoint_2r> vertices_;
    Visual::Material mat_vertex_;
    Visual::Material mat_edge_;
    int32_t z_order_;
    bool closed_;
};

//=============================================================================
// Interface: Polygon_2r
//=============================================================================

class Polygon_2r : public Visual::Geometry {
public:
    Polygon_2r();
    ~Polygon_2r();

    void push_back(const Point_2r& v);
    void push_back(SharedPoint_2r v);
    void pop_back();
    SharedPoint_2r back(const size_t i = 0) const;

    void push_front(const Point_2r& v);
    void push_front(SharedPoint_2r v);
    void pop_front();
    SharedPoint_2r front(const size_t i = 0) const;

    SharedPoint_2r operator[](const size_t i) const;
    const size_t size() const;

    void CloseBoundary();

    const Polyline_2r& boundary() const;

    void set_mat_vertex(const Visual::Material mat_vertex) { boundary_.set_mat_vertex(mat_vertex); }
    void set_mat_edge(const Visual::Material mat_edge) { boundary_.set_mat_edge(mat_edge); }
    void set_mat_face(const Visual::Material mat_face) { mat_face_ = mat_face; }
    void set_z_order(const uint32_t z_order) { z_order_ = z_order; boundary_.set_z_order(z_order); }

private:
    Polyline_2r boundary_;
    Visual::Material mat_face_;
    uint32_t z_order_;
};

Polygon_2r Melkman(const Polyline_2r& P, Visual::IGeometryObserver* observer = nullptr);
Polygon_2r IntegerHull(const Polygon_2r& P, Visual::IGeometryObserver* observer = nullptr);

} // namespace DDAD

#endif // GE_POLYGON_H
