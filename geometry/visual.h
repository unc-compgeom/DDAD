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

#ifndef GE_VISUAL_H
#define GE_VISUAL_H

#include "common.h"
#include "point.h"
#include "line.h"
#include "triangle.h"

namespace DDAD {

namespace Visual {

//=============================================================================
// Interface: Color
//=============================================================================

class Color {
public:
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color BLACK;
    static const Color GRAY;
    static const Color WHITE;
    static const Color CYAN;
    static const Color YELLOW;
    static const Color MAGENTA;
    static const Color SKYBLUE;

    Color();
    Color(const unsigned char r, const unsigned char g,
          const unsigned char b, const unsigned char a);

    const unsigned char r() const;
    const unsigned char g() const;
    const unsigned char b() const;
    const unsigned char a() const;
    const std::array<unsigned char, 4>& rgba() const;
    void set_r(const unsigned char r);
    void set_g(const unsigned char g);
    void set_b(const unsigned char b);
    void set_a(const unsigned char a);
    void set_rgba(const std::array<unsigned char, 4>& rgba);

private:
    std::array<unsigned char, 4> rgba_;
};

//=============================================================================
// Interface: Material
//=============================================================================

namespace Coverage {
    enum Type {
        E_OPAQUE,
        E_TRANSPARENT,
        E_MAX
    };
}

namespace Lighting {
    enum Type {
        E_UNLIT,
        E_FLAT,
        E_MAX
    };
}

class Material {
public:
    Material();
    Material(const Color& ambient, const Color& diffuse,
             const Color& specular);
    Material(const Color& ambient, const Color& diffuse,
             const Color& specular, Coverage::Type coverage,
             Lighting::Type lighting);

    const Color& ambient() const;
    const Color& diffuse() const;
    const Color& specular() const;
    Coverage::Type coverage() const;
    Lighting::Type lighting() const;
    void set_ambient(const Color& ambient);
    void set_diffuse(const Color& diffuse);
    void set_specular(const Color& specular);
    void set_coverage(Coverage::Type coverage);
    void set_lighting(Lighting::Type lighting);

private:
    Color ambient_;
    Color diffuse_;
    Color specular_;
    Coverage::Type coverage_;
    Lighting::Type lighting_;
};

//=============================================================================
// Interface: Point
//=============================================================================

class Point {
public:
    Point();
    Point(const Material& material, const int32_t z_order = 0);

    const Material& material() const;
    void set_material(const Material& material);
    const int32_t z_order() const { return z_order_; }
    void set_z_order(const int32_t z_order) { z_order_ = z_order; }

private:
    Material material_;
    std::string sprite_;
    int32_t z_order_;
};

//=============================================================================
// Interface: Segment
//=============================================================================

class Segment {
public:
    Segment();
    Segment(const Material& material);

    const Material& material() const;
    void set_material(const Material& material);

private:
    Material material_;
};

//=============================================================================
// Interface: Triangle
//=============================================================================

class Triangle {
public:
    Triangle();
    Triangle(const Material& material);

    const Material& material() const;
    void set_material(const Material& material);

private:
    Material material_;
};

//=============================================================================
// Interface: IGeometryObserver
//=============================================================================

struct IGeometryObserver {
    virtual void SlotRegisterPoint_2r(Point_2r& p) = 0;

    virtual void SlotPushVisualPoint_2r(const Point_2r& p,
                                        const Visual::Point& vp,
                                        const uint32_t msec_delay) = 0;

    virtual void SlotPushVisualSegment_2r(const Segment_2r& s,
                                          const Visual::Segment& vs,
                                          const uint32_t msec_delay) = 0;

    virtual void SlotPushVisualTriangle_2r(const Triangle_2r& t,
                                           const Visual::Triangle& vt,
                                           const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualPoint_2r(const Point_2r& p,
                                       const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualSegment_2r(const Segment_2r& s,
                                      const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualTriangle_2r(const Triangle_2r& t,
                                          const uint32_t msec_delay) = 0;

    virtual void SlotRegisterPoint_3r(Point_3r& p) = 0;

    virtual void SlotPushVisualPoint_3r(const Point_3r& p,
                                        const Visual::Point& vp,
                                        const uint32_t msec_delay) = 0;

    virtual void SlotPushVisualSegment_3r(const Segment_3r& s,
                                          const Visual::Segment& vs,
                                          const uint32_t msec_delay) = 0;

    virtual void SlotPushVisualTriangle_3r(const Triangle_3r& t,
                                           const Visual::Triangle& vt,
                                           const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualPoint_3r(const Point_3r& p,
                                       const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualSegment_3r(const Segment_3r& s,
                                      const uint32_t msec_delay) = 0;

    virtual void SlotPopVisualTriangle_3r(const Triangle_3r& t,
                                          const uint32_t msec_delay) = 0;

    virtual void SlotUpdate() = 0;
};

//=============================================================================
// Interface: IObservableGeometry
//=============================================================================

struct IObservableGeometry {
    virtual void AddObserver(IGeometryObserver* geom_observer) = 0;
    virtual void RemoveObserver(IGeometryObserver* geom_observer) = 0;

    virtual void SigRegisterPoint_2r(Point_2r& p) = 0;

    virtual void SigPushVisualPoint_2r(const Point_2r& p,
                                       const Visual::Point& vp,
                                       const uint32_t msec_delay) const = 0;

    virtual void SigPushVisualSegment_2r(const Segment_2r& s,
                                         const Visual::Segment& vs,
                                         const uint32_t msec_delay) const = 0;

    virtual void SigPushVisualTriangle_2r(const Triangle_2r& t,
                                          const Visual::Triangle& vt,
                                          const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualPoint_2r(const Point_2r& p,
                                      const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualSegment_2r(const Segment_2r& s,
                                        const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualTriangle_2r(const Triangle_2r& t,
                                         const uint32_t msec_delay) const = 0;

    virtual void SigRegisterPoint_3r(Point_3r& p) = 0;

    virtual void SigPushVisualPoint_3r(const Point_3r& p,
                                       const Visual::Point& vp,
                                       const uint32_t msec_delay) const = 0;

    virtual void SigPushVisualSegment_3r(const Segment_3r& s,
                                         const Visual::Segment& vs,
                                         const uint32_t msec_delay) const = 0;

    virtual void SigPushVisualTriangle_3r(const Triangle_3r& t,
                                          const Visual::Triangle& vt,
                                          const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualPoint_3r(const Point_3r& p,
                                      const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualSegment_3r(const Segment_3r& s,
                                        const uint32_t msec_delay) const = 0;

    virtual void SigPopVisualTriangle_3r(const Triangle_3r& t,
                                         const uint32_t msec_delay) const = 0;

    virtual void SigUpdate() const = 0;
};

class Geometry : public IGeometryObserver, public IObservableGeometry {
public:
    virtual ~Geometry();

    /* Observer */

    void SlotRegisterPoint_2r(Point_2r& p) override;

    void SlotPushVisualPoint_2r(const Point_2r& p,
                                const Visual::Point& vp,
                                const uint32_t msec_delay = 0) override;

    void SlotPushVisualSegment_2r(const Segment_2r& s,
                                  const Visual::Segment& vs,
                                  const uint32_t msec_delay = 0) override;

    void SlotPushVisualTriangle_2r(const Triangle_2r& t,
                                   const Visual::Triangle& vt,
                                   const uint32_t msec_delay = 0) override;

    void SlotPopVisualPoint_2r(const Point_2r& p,
                               const uint32_t msec_delay = 0) override;

    void SlotPopVisualSegment_2r(const Segment_2r& s,
                                 const uint32_t msec_delay = 0) override;

    void SlotPopVisualTriangle_2r(const Triangle_2r& t,
                                  const uint32_t msec_delay = 0) override;


    void SlotRegisterPoint_3r(Point_3r& p) override;

    void SlotPushVisualPoint_3r(const Point_3r& p,
                                const Visual::Point& vp,
                                const uint32_t msec_delay = 0) override;

    void SlotPushVisualSegment_3r(const Segment_3r& s,
                                  const Visual::Segment& vs,
                                  const uint32_t msec_delay = 0) override;

    void SlotPushVisualTriangle_3r(const Triangle_3r& t,
                                   const Visual::Triangle& vt,
                                   const uint32_t msec_delay = 0) override;

    void SlotPopVisualPoint_3r(const Point_3r& p,
                               const uint32_t msec_delay = 0) override;

    void SlotPopVisualSegment_3r(const Segment_3r& s,
                                 const uint32_t msec_delay = 0) override;

    void SlotPopVisualTriangle_3r(const Triangle_3r& t,
                                  const uint32_t msec_delay = 0) override;


    void SlotUpdate() override;

    /* Observable */

    void AddObserver(IGeometryObserver* geom_observer) override;
    void RemoveObserver(IGeometryObserver* geom_observer) override;

    void SigRegisterPoint_2r(Point_2r& p) override;

    void SigPushVisualPoint_2r(const Point_2r& p, const Visual::Point& vp,
                            const uint32_t msec_delay = 0) const override;

    void SigPushVisualSegment_2r(const Segment_2r& s, const Visual::Segment& vs,
                              const uint32_t msec_delay = 0) const override;

    void SigPushVisualTriangle_2r(const Triangle_2r& t, const Visual::Triangle& vt,
                               const uint32_t msec_delay = 0) const override;

    void SigPopVisualPoint_2r(const Point_2r& p,
                           const uint32_t msec_delay = 0) const override;

    void SigPopVisualSegment_2r(const Segment_2r& s,
                             const uint32_t msec_delay = 0) const override;

    void SigPopVisualTriangle_2r(const Triangle_2r& t,
                             const uint32_t msec_delay = 0) const override;

    void SigRegisterPoint_3r(Point_3r& p) override;

    void SigPushVisualPoint_3r(const Point_3r& p, const Visual::Point& vp,
                               const uint32_t msec_delay = 0) const override;

    void SigPushVisualSegment_3r(const Segment_3r& s, const Visual::Segment& vs,
                                 const uint32_t msec_delay = 0) const override;

    void SigPushVisualTriangle_3r(const Triangle_3r& t,
                                  const Visual::Triangle& vt,
                                  const uint32_t msec_delay = 0) const override;

    void SigPopVisualPoint_3r(const Point_3r& p,
                           const uint32_t msec_delay = 0) const override;

    void SigPopVisualSegment_3r(const Segment_3r& s,
                                const uint32_t msec_delay = 0) const override;

    void SigPopVisualTriangle_3r(const Triangle_3r& t,
                                 const uint32_t msec_delay = 0) const override;

    void SigUpdate() const override;

protected:
    std::vector<IGeometryObserver*> observers_;
};

} // namespace Visual

} // namespace DDAD

#endif // GE_VISUAL_H
