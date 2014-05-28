/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-08-14
 */

#include "common.h"
#include "arithmetic.h"
#include "triangle.h"

namespace DDAD {

//=============================================================================
// Implementation: Triangle_2r
//=============================================================================

Triangle_2r::Triangle_2r() {}

Triangle_2r::Triangle_2r(SharedPoint_2r a,
                         SharedPoint_2r b,
                         SharedPoint_2r c) :
    a_(a),
    b_(b),
    c_(c) {}

std::ostream& operator<<(std::ostream& o, const Triangle_2r& tri) {
    return o << "a: " << tri.a() << "\nb: " << tri.b() << "\nc: " << tri.c();
}

const Point_2r& Triangle_2r::a() const {
    return *a_;
}
const Point_2r& Triangle_2r::b() const {
    return *b_;
}
const Point_2r& Triangle_2r::c() const {
    return *c_;
}
void Triangle_2r::set_a(SharedPoint_2r a) {
    a_ = a;
}
void Triangle_2r::set_b(SharedPoint_2r b) {
    b_ = b;
}
void Triangle_2r::set_c(SharedPoint_2r c) {
    c_ = c;
}
SharedPoint_2r Triangle_2r::a_sptr() {
    return a_;
}
SharedPoint_2r Triangle_2r::b_sptr() {
    return b_;
}
SharedPoint_2r Triangle_2r::c_sptr() {
    return c_;
}

//=============================================================================
// Implementation: Triangle_3r
//=============================================================================

Triangle_3r::Triangle_3r() {}

Triangle_3r::Triangle_3r(SharedPoint_3r a, SharedPoint_3r b, SharedPoint_3r c) :
    a_(a),
    b_(b),
    c_(c) {}

std::ostream& operator<<(std::ostream& o, const Triangle_3r& tri) {
    return o << "a: " << tri.a() << "\nb: " << tri.b() << "\nc: " << tri.c();
}

const Point_3r& Triangle_3r::a() const {
    return *a_;
}
const Point_3r& Triangle_3r::b() const {
    return *b_;
}
const Point_3r& Triangle_3r::c() const {
    return *c_;
}
void Triangle_3r::set_a(SharedPoint_3r a) {
    a_ = a;
}
void Triangle_3r::set_b(SharedPoint_3r b) {
    b_ = b;
}
void Triangle_3r::set_c(SharedPoint_3r c) {
    c_ = c;
}
SharedPoint_3r Triangle_3r::a_sptr() {
    return a_;
}
SharedPoint_3r Triangle_3r::b_sptr() {
    return b_;
}
SharedPoint_3r Triangle_3r::c_sptr() {
    return c_;
}

} // namespace DDAD
