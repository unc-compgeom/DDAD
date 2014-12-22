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

#ifndef GE_WEDGE_H
#define GE_WEDGE_H

#include "common.h"
#include "visual.h"
#include "point.h"
#include "vector.h"
#include "triangle.h"

namespace DDAD {

//=============================================================================
// Interface: Wedge_2r
//=============================================================================

class Wedge_2r : public Visual::Geometry {
public:
    Wedge_2r();
    Wedge_2r(const Vector_2r& u, const Vector_2r& v);
    ~Wedge_2r();

    void AttachToOrigin(SharedPoint_2r origin);

    const Vector_2r& u() const;
    const Vector_2r& v() const;
    const Point_2r& ou() const { return *ou_; }
    const Point_2r& ov() const { return *ov_; }
    const Point_2r& ouv() const { return *ouv_; }
    SharedPoint_2r ou_sptr() { return ou_; }
    SharedPoint_2r ov_sptr() { return ov_; }
    SharedPoint_2r ouv_sptr() { return ouv_; }
    const Segment_2r& u_segment() const;
    const Segment_2r& v_segment() const;
    const Triangle_2r& u_tri() const { return u_tri_; }
    const Triangle_2r& v_tri() const { return v_tri_; }
    const Visual::Color& diffuse() const { return diffuse_; }
    bool attached() const;

private:
    Vector_2r u_;
    Vector_2r v_;
    Segment_2r u_segment_;
    Segment_2r v_segment_;
    Triangle_2r u_tri_;
    Triangle_2r v_tri_;
    SharedPoint_2r o_;
    SharedPoint_2r ou_;
    SharedPoint_2r ov_;
    SharedPoint_2r ouv_;
    bool attached_;
    Visual::Color diffuse_;
};

//=============================================================================
// Interface: WedgeStack_2r
//=============================================================================

class WedgeStack_2r : public Visual::Geometry {
public:
    WedgeStack_2r();
    WedgeStack_2r(SharedPoint_2r origin);
    ~WedgeStack_2r();

    /*
    void Push(const Wedge_2r& w);
    Wedge_2r Pop();
    const Wedge_2r& Top() const;
    */

    void Push(std::shared_ptr<Wedge_2r> w);
    std::shared_ptr<Wedge_2r> Pop();
    std::shared_ptr<Wedge_2r> Top();
    bool empty() const;

    void AttachToOrigin(SharedPoint_2r origin);

private:
    //std::vector<Wedge_2r> wedge_stack_;
    std::vector<std::shared_ptr<Wedge_2r>> wedge_stack_;
    SharedPoint_2r origin_;
};

} // namespace DDAD

#endif // GE_WEDGE_H
