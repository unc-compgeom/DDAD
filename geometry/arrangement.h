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
#include "datastructures.h"

namespace DDAD {




//=============================================================================
// Interface: Arrangement_Vertex_2r
//=============================================================================

class Arrangement_Vertex_2r : public Point_2r {
public:
    Arrangement_Vertex_2r();
//    ~Arrangement_Vertex_2r();
    Arrangement_Vertex_2r(Point_2r& pt, SharedPoint_2r otherPt, bool color);

    const Point_2r location() const;
    const bool getColor();
    SharedPoint_2r getOtherPoint();
    const rational& getX() { return location_.x(); }
    const rational& getY() { return location_.y(); }


private:
    bool color_;
    SharedPoint_2r otherPoint_;
    Point_2r location_;
};

//=============================================================================
// Interface: Arrangement_Bundle
//=============================================================================

class Arrangement_Bundle : public SplayTree<Segment_2r_colored>{
public:
    Arrangement_Bundle();
    // Checks whether an input segment is both below the top segment and above the bottom segment in the bundle
    bool contains(const Arrangement_Vertex_2r& vert);
    // Make sure that segments are inserted with correct orientation
    void insert(const Segment_2r_colored &x);

};

//=============================================================================
// Interface: Bundle_Tree
//=============================================================================

class Bundle_Tree {
public:
    Bundle_Tree();


};

//=============================================================================
// Interface: Bundle_List
//=============================================================================

class Bundle_List {
public:
    Bundle_List();


private:
    std::list<Arrangement_Bundle> blist_;
};

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

    const std::list<Arrangement_Vertex_2r>& vertices() const;
    const std::list<Segment_2r_colored>& segments() const;



private:
    std::list<Arrangement_Vertex_2r> vertices_;
    std::list<Segment_2r_colored> segments_;
    Point_2r floater_;
    bool current_color_;
};

int CountIntersections(const Arrangement_2r& A,
                       Visual::IGeometryObserver* observer = nullptr);


} // namespace DDAD

#endif // GE_ARRANGEMENT_H
