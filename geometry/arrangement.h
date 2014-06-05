/*!
 * @brief 2-dimensional arrangement of segments.
 * @author Sarah Wright <s.wright.618@gmail.com>
 * @date 06/04/2014
 */

#ifndef GE_ARRANGEMENT_H
#define GE_ARRANGEMENT_H

#include "common.h"
#include "visual.h"
#include "point.h"
#include "line.h"

namespace DDAD {

//=============================================================================
// Interface: ArrangementVertex_2r
//=============================================================================

class ArrangementVertex_2r {
public:
    ArrangementVertex_2r();
    explicit ArrangementVertex_2r(const Point_2r& vertex);
    explicit ArrangementVertex_2r(SharedPoint_2r vertex);

    const Point_2r& vertex() const;
    Point_2r& vertex();
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
    bool AIsLeftOfB(const ArrangementVertex_2r& a, const ArrangementVertex_2r& b);
    bool AIsBelowB(const ArrangementVertex_2r& a, const ArrangementVertex_2r& b);
}

//=============================================================================
// Interface: Arrangement_2r
//=============================================================================

class Arrangement_2r : public Visual::Geometry {
public:
//    Arrangement_2r();
    ~Arrangement_2r();

    void AddSegment(Point_2r& v, Point_2r& w, bool color);
    void EndSegment(Point_2r& v);
    void PushPoint(Point_2r& v, bool color);
    void PushPoint(SharedPoint_2r v, bool color);
    void PopPoint();

    const std::list<Segment_2r_colored>& segments() const;


private:
    std::list<Segment_2r_colored> segments_;
    Point_2r floater_;
    bool current_color_;
};

} // namespace DDAD

#endif // GE_ARRANGEMENT_H
