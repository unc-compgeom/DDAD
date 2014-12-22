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
#include "line.h"
#include "wedge.h"

namespace DDAD {

//=============================================================================
// Wedge_2r
//=============================================================================

Wedge_2r::Wedge_2r() :
    attached_(false),
    diffuse_(0, 0, 0, 100) {}

Wedge_2r::Wedge_2r(const Vector_2r& u, const Vector_2r& v) :
    u_(u),
    v_(v),
    attached_(false),
    diffuse_(0, 0, 0, 100) {}

Wedge_2r::~Wedge_2r() {}

void Wedge_2r::AttachToOrigin(SharedPoint_2r o) {
    if(!attached_) {
        o_ = o;
        ou_ = std::make_shared<Point_2r>(*o_+u_);
        ov_ = std::make_shared<Point_2r>(*o_+v_);
        ouv_ = std::make_shared<Point_2r>(*o_+u_+v_);
        SigRegisterPoint_2r(*ou_);
        SigRegisterPoint_2r(*ov_);
        SigRegisterPoint_2r(*ouv_);
        u_segment_ = Segment_2r(o_, ou_);
        v_segment_ = Segment_2r(o_, ov_);
        u_tri_.set_a(o_);
        u_tri_.set_b(ou_);
        u_tri_.set_c(ouv_);
        v_tri_.set_a(o_);
        v_tri_.set_b(ouv_);
        v_tri_.set_c(ov_);
        attached_ = true;
    } else {
        SigPopVisualSegment_2r(u_segment_);
        SigPopVisualSegment_2r(v_segment_);
        SigPopVisualTriangle_2r(u_tri_);
        SigPopVisualTriangle_2r(v_tri_);

        o_ = o;
        ou_->set_elements((*o_+u_).elements());
        ou_->SigPositionChanged();
        ov_->set_elements((*o_+v_).elements());
        ov_->SigPositionChanged();
        ouv_->set_elements((*o_+u_+v_).elements());
        ouv_->SigPositionChanged();

        u_segment_ = Segment_2r(o_, ou_);
        v_segment_ = Segment_2r(o_, ov_);
        u_tri_.set_a(o_);
        v_tri_.set_a(o_);

        Visual::Triangle vt;
        //vt.set_diffuse(diffuse_);
        SigPushVisualTriangle_2r(u_tri_, vt);
        SigPushVisualTriangle_2r(v_tri_, vt);
        //SigPushVisualSegment(u_segment_, RSF_ACTIVE);
        //SigPushVisualSegment(v_segment_, RSF_ACTIVE);

        /*
        u_segment_.set_p(o_);
        u_segment_.q_sptr()->set_elements((u_segment_.p()+u_).elements());
        u_segment_.q().SignalPositionChanged();
        SignalPushSegmentFlags(u_segment_, RSF_ACTIVE);
        v_segment_.set_p(o);
        v_segment_.q_sptr()->set_elements((v_segment_.p()+v_).elements());
        v_segment_.q().SignalPositionChanged();
        SignalPushSegmentFlags(v_segment_, RSF_ACTIVE);
        */
    }
}

// Accessors/Mutators =========================================================

const Vector_2r& Wedge_2r::u() const {
    return u_;
}
const Vector_2r& Wedge_2r::v() const {
    return v_;
}
const Segment_2r& Wedge_2r::u_segment() const {
    return u_segment_;
}
const Segment_2r& Wedge_2r::v_segment() const {
    return v_segment_;
}
bool Wedge_2r::attached() const {
    return attached_;
}

//=============================================================================
// WedgeStack_2r
//=============================================================================

WedgeStack_2r::WedgeStack_2r() {}

WedgeStack_2r::WedgeStack_2r(SharedPoint_2r origin) :
    origin_(origin) {}

WedgeStack_2r::~WedgeStack_2r() {}

//! \note assumes origin is already registered
void WedgeStack_2r::AttachToOrigin(SharedPoint_2r origin) {
    origin_ = origin;
    if(!origin_->unique_id()) {
        SigRegisterPoint_2r(*origin_);
    }
    for(auto i = begin(wedge_stack_); i != end(wedge_stack_); ++i) {
        (*i)->AttachToOrigin(origin);
    }
}

/*!
 * @brief Pushes a new wedge onto the stack.
 * \param w - wedge to be pushed
 */
void WedgeStack_2r::Push(std::shared_ptr<Wedge_2r> w) {
    // actually push it
    wedge_stack_.push_back(w);

    // start watching the wedge for signals
    wedge_stack_.back()->AddObserver(this);

    // inform the wedge of where this stack is located
    if(origin_) {
        wedge_stack_.back()->AttachToOrigin(origin_);
    } else {
        //rError("wedge stack not attached to origin.");
    }

    // visualize u and v vectors and interior parallelogram
    /*
    SignalPushSegmentFlags(wedge_stack_.back()->u_segment(), RSF_ACTIVE);
    SignalPushSegmentFlags(wedge_stack_.back()->v_segment(), RSF_ACTIVE);
    VisualTriangle vt;
    vt.set_diffuse(wedge_stack_.back()->diffuse());
    SignalPushTriangleState(wedge_stack_.back()->u_tri(), vt);
    SignalPushTriangleState(wedge_stack_.back()->v_tri(), vt, 500);
    */
    //SignalPushSegmentFlags(wedge_stack_.back()->u_segment(), RSF_ACTIVE);
    //SignalPushSegmentFlags(wedge_stack_.back()->v_segment(), RSF_ACTIVE, 1000);
}

/*!
 * @brief Removes and returns the top element on the stack.
 * \return wedge on top of stack
 */
std::shared_ptr<Wedge_2r> WedgeStack_2r::Pop() {
    /*
    SignalPopSegmentFlags(wedge_stack_.back()->u_segment());
    SignalPopSegmentFlags(wedge_stack_.back()->v_segment());
    SignalPopTriangleState(wedge_stack_.back()->u_tri());
    SignalPopTriangleState(wedge_stack_.back()->v_tri());
    */
    // remove visualization of u and v vectors and interior parallelogram
    //SignalPopSegmentFlags(wedge_stack_.back()->u_segment());
    //SignalPopSegmentFlags(wedge_stack_.back()->v_segment());

    // grab a copy, pop, and return
    auto out = wedge_stack_.back();
    wedge_stack_.pop_back();
    return out;
}

std::shared_ptr<Wedge_2r> WedgeStack_2r::Top() {
    return wedge_stack_.back();
}

bool WedgeStack_2r::empty() const {
    return wedge_stack_.empty();
}

} // namespace DDAD
