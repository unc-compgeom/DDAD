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
#include "visual.h"

namespace DDAD {

namespace Visual {

Geometry::~Geometry() {}

void Geometry::SlotRegisterPoint_2r(Point_2r& p) {
    SigRegisterPoint_2r(p);
}

void Geometry::SlotPushVisualPoint_2r(const Point_2r& p,
                                      const Visual::Point& vp,
                                      const uint32_t msec_delay) {
    SigPushVisualPoint_2r(p, vp, msec_delay);
}

void Geometry::SlotPushVisualSegment_2r(const Segment_2r& s,
                                        const Visual::Segment& vs,
                                        const uint32_t msec_delay) {
    SigPushVisualSegment_2r(s, vs, msec_delay);
}

void Geometry::SlotPushVisualTriangle_2r(const Triangle_2r &t,
                                         const Visual::Triangle& vt,
                                         const uint32_t msec_delay) {
    SigPushVisualTriangle_2r(t, vt, msec_delay);
}

void Geometry::SlotPopVisualPoint_2r(const Point_2r& p,
                                     const uint32_t msec_delay) {
    SigPopVisualPoint_2r(p, msec_delay);
}

void Geometry::SlotPopVisualSegment_2r(const Segment_2r& s,
                                       const uint32_t msec_delay) {
    SigPopVisualSegment_2r(s, msec_delay);
}

void Geometry::SlotPopVisualTriangle_2r(const Triangle_2r &t,
                                        const uint32_t msec_delay) {
    SigPopVisualTriangle_2r(t, msec_delay);
}

void Geometry::SlotRegisterPoint_3r(Point_3r& p) {
    SigRegisterPoint_3r(p);
}

void Geometry::SlotPushVisualPoint_3r(const Point_3r& p,
                                      const Visual::Point& vp,
                                      const uint32_t msec_delay) {
    SigPushVisualPoint_3r(p, vp, msec_delay);
}

void Geometry::SlotPushVisualSegment_3r(const Segment_3r& s,
                                        const Visual::Segment& vs,
                                        const uint32_t msec_delay) {
    SigPushVisualSegment_3r(s, vs, msec_delay);
}

void Geometry::SlotPushVisualTriangle_3r(const Triangle_3r &t,
                                         const Visual::Triangle& vt,
                                         const uint32_t msec_delay) {
    SigPushVisualTriangle_3r(t, vt, msec_delay);
}

void Geometry::SlotPopVisualPoint_3r(const Point_3r& p,
                                     const uint32_t msec_delay) {
    SigPopVisualPoint_3r(p, msec_delay);
}

void Geometry::SlotPopVisualSegment_3r(const Segment_3r& s,
                                       const uint32_t msec_delay) {
    SigPopVisualSegment_3r(s, msec_delay);
}

void Geometry::SlotPopVisualTriangle_3r(const Triangle_3r &t,
                                        const uint32_t msec_delay) {
    SigPopVisualTriangle_3r(t, msec_delay);
}

void Geometry::SlotUpdate() {
    SigUpdate();
}

void Geometry::AddObserver(IGeometryObserver* geom_observer) {
    observers_.push_back(geom_observer);
}

void Geometry::RemoveObserver(IGeometryObserver* geom_observer) {
    observers_.erase(std::remove(begin(observers_), end(observers_),
        geom_observer), end(observers_));
}

void Geometry::SigRegisterPoint_2r(Point_2r &p) {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotRegisterPoint_2r(p);
    }
}

void Geometry::SigPushVisualPoint_2r(const Point_2r& p,
                                     const Visual::Point& vp,
                                     const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualPoint_2r(p, vp, msec_delay);
    }
}

void Geometry::SigPushVisualSegment_2r(const Segment_2r& s,
                                       const Visual::Segment& vs,
                                       const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualSegment_2r(s, vs, msec_delay);
    }
}

void Geometry::SigPushVisualTriangle_2r(const Triangle_2r& t,
                                        const Visual::Triangle& vt,
                                        const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualTriangle_2r(t, vt, msec_delay);
    }
}

void Geometry::SigPopVisualPoint_2r(const Point_2r& p,
                                    const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualPoint_2r(p, msec_delay);
    }
}

void Geometry::SigPopVisualSegment_2r(const Segment_2r& s,
                                      const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualSegment_2r(s, msec_delay);
    }
}

void Geometry::SigPopVisualTriangle_2r(const Triangle_2r &t,
                                       const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualTriangle_2r(t, msec_delay);
    }
}

void Geometry::SigRegisterPoint_3r(Point_3r &p) {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotRegisterPoint_3r(p);
    }
}

void Geometry::SigPushVisualPoint_3r(const Point_3r& p,
                                     const Visual::Point& vp,
                                     const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualPoint_3r(p, vp, msec_delay);
    }
}

void Geometry::SigPushVisualSegment_3r(const Segment_3r& s,
                                       const Visual::Segment& vs,
                                       const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualSegment_3r(s, vs, msec_delay);
    }
}

void Geometry::SigPushVisualTriangle_3r(const Triangle_3r& t,
                                        const Visual::Triangle& vt,
                                        const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPushVisualTriangle_3r(t, vt, msec_delay);
    }
}

void Geometry::SigPopVisualPoint_3r(const Point_3r& p,
                                    const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualPoint_3r(p, msec_delay);
    }
}

void Geometry::SigPopVisualSegment_3r(const Segment_3r& s,
                                      const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualSegment_3r(s, msec_delay);
    }
}

void Geometry::SigPopVisualTriangle_3r(const Triangle_3r &t,
                                       const uint32_t msec_delay) const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPopVisualTriangle_3r(t, msec_delay);
    }
}

void Geometry::SigUpdate() const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotUpdate();
    }
}

//=============================================================================
// Implementation: Color
//=============================================================================

const Color Color::RED = Color(255, 0, 0, 255);
const Color Color::GREEN = Color(0, 255, 0, 255);
const Color Color::BLUE = Color(0, 0, 255, 255);
const Color Color::BLACK = Color(0, 0, 0, 255);
const Color Color::GRAY = Color(175, 175, 175, 255);
const Color Color::WHITE = Color(255, 255, 255, 255);
const Color Color::CYAN = Color(0, 255, 255, 255);
const Color Color::YELLOW = Color(255, 255, 0, 255);
const Color Color::MAGENTA = Color(255, 0, 255, 255);
const Color Color::SKYBLUE = Color(0, 151, 255, 255);

Color::Color() {
    rgba_.assign(0);
    rgba_[3] = 255;
}

Color::Color(const unsigned char r, const unsigned char g,
             const unsigned char b, const unsigned char a) {
    rgba_[0] = r;
    rgba_[1] = g;
    rgba_[2] = b;
    rgba_[3] = a;
}

const unsigned char Color::r() const {
    return rgba_[0];
}
const unsigned char Color::g() const {
    return rgba_[1];
}
const unsigned char Color::b() const {
    return rgba_[2];
}
const unsigned char Color::a() const {
    return rgba_[3];
}
const std::array<unsigned char, 4>& Color::rgba() const {
    return rgba_;
}
void Color::set_r(const unsigned char r) {
    rgba_[0] = r;
}
void Color::set_g(const unsigned char g) {
    rgba_[1] = g;
}
void Color::set_b(const unsigned char b) {
    rgba_[2] = b;
}
void Color::set_a(const unsigned char a) {
    rgba_[3] = a;
}
void Color::set_rgba(const std::array<unsigned char, 4>& rgba) {
    rgba_ = rgba;
}

//=============================================================================
// Implementation: Material
//=============================================================================

Material::Material() :
    coverage_(Coverage::E_OPAQUE),
    lighting_(Lighting::E_UNLIT) {}

Material::Material(const Color& ambient, const Color& diffuse,
                   const Color& specular) :
    ambient_(ambient),
    diffuse_(diffuse),
    specular_(specular),
    coverage_(Coverage::E_OPAQUE),
    lighting_(Lighting::E_UNLIT) {}

Material::Material(const Color& ambient, const Color& diffuse,
                   const Color& specular, Coverage::Type coverage,
                   Lighting::Type lighting) :
    ambient_(ambient),
    diffuse_(diffuse),
    specular_(specular),
    coverage_(coverage),
    lighting_(lighting) {}

const Color& Material::ambient() const {
    return ambient_;
}
const Color& Material::diffuse() const {
    return diffuse_;
}
const Color& Material::specular() const {
    return specular_;
}
Coverage::Type Material::coverage() const {
    return coverage_;
}
Lighting::Type Material::lighting() const {
    return lighting_;
}
void Material::set_ambient(const Color& ambient) {
    ambient_ = ambient;
}
void Material::set_diffuse(const Color& diffuse) {
    diffuse_ = diffuse;
}
void Material::set_specular(const Color& specular) {
    specular_ = specular;
}
void Material::set_coverage(Coverage::Type coverage) {
    coverage_ = coverage;
}
void Material::set_lighting(Lighting::Type lighting) {
    lighting_ = lighting;
}

//=============================================================================
// Implementation: Point
//=============================================================================

Point::Point() :
    z_order_(0) {}

Point::Point(const Material& material, const int32_t z_order) :
    material_(material),
    z_order_(z_order) {}

const Material& Point::material() const {
    return material_;
}
void Point::set_material(const Material& material) {
    material_ = material;
}

//=============================================================================
// Implementation: Segment
//=============================================================================

Segment::Segment() {}

Segment::Segment(const Material& material) :
    material_(material) {}

const Material& Segment::material() const {
    return material_;
}
void Segment::set_material(const Material& material) {
    material_ = material;
}

//=============================================================================
// Implementation: Triangle
//=============================================================================

Triangle::Triangle() {}

Triangle::Triangle(const Material& material) :
    material_(material) {}

const Material& Triangle::material() const {
    return material_;
}
void Triangle::set_material(const Material& material) {
    material_ = material;
}

} // namespace Visual

} // namespace DDAD
