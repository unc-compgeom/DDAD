/*!
 * @brief 2-dimensional arrangement of segments.
 * @author Sarah Wright <s.wright.618@gmail.com>
 * @date 06/04/2014
 */

#ifndef GE_ARRANGEMENT_H
#define GE_ARRANGEMENT_H

//#include "common.h"
#include "visual.h"
#include "point.h"
#include "line.h"
#include "datastructures.h"

namespace DDAD {

enum RelativePosition{
    ABOVE = 3,
    ENDING = 2,
    BELOW = 1,
    INDETERMINATE = 0
};


//=============================================================================
// Interface: Arrangement_Vertex_2r
//=============================================================================

class Arrangement_Vertex_2r {
public:
    Arrangement_Vertex_2r();
    Arrangement_Vertex_2r(SharedPoint_2r &pt, SharedPoint_2r otherPt, bool color);

    const Point_2r position() const { return *pt_; }
    const bool getColor() { return color_; }
    SharedPoint_2r getOtherPoint() const {return otherPoint_; }
    SharedPoint_2r getPoint() const {return pt_; }
    const rational& getX() { return pt_->x(); }
    const rational& getY() { return pt_->y(); }


private:
    bool color_;
    SharedPoint_2r otherPoint_;
    SharedPoint_2r pt_;
};

//=============================================================================
// Interface: Arrangement_Bundle
//=============================================================================

class Arrangement_Bundle : public SplayTree<Segment_2r_colored>{
public:
    Arrangement_Bundle();
    Arrangement_Bundle(SplayTree<Segment_2r_colored> &rhs);
    // Checks whether an input segment is both below the top segment and above the bottom segment in the bundle
    bool contains(const Arrangement_Vertex_2r& vert);
    // Make sure that segments are inserted with correct orientation
    void insert(const Segment_2r_colored &x);
    // Bundles need to be able to tell whether they are "above", "ending at", or "below" a given point
    RelativePosition relPosition(const Arrangement_Vertex_2r &p);
    Segment_2r_colored* getBot() const { return botSegment_; }
    Segment_2r_colored* getTop() const { return topSegment_; }
    void SplitBundleAtSegment(Segment_2r_colored &pivot_segment);
private:
    Segment_2r_colored* topSegment_;
    Segment_2r_colored* botSegment_;

};
bool operator<(const Arrangement_Bundle &lhs, const Arrangement_Bundle &rhs);
bool operator>(const Arrangement_Bundle &lhs, const Arrangement_Bundle &rhs);
bool operator==(const Arrangement_Bundle &lhs, const Arrangement_Bundle &rhs);
bool operator!=(const Arrangement_Bundle &lhs, const Arrangement_Bundle &rhs);

//=============================================================================
// Interface: Bundle_Tree
//=============================================================================

class Bundle_Tree : public SplayTree<Arrangement_Bundle>{
public:
    //Bundle_Tree();
    void LocatePoint(Arrangement_Vertex_2r &p, Arrangement_Bundle &top, Arrangement_Bundle &bottom);
    void splitBundles(Arrangement_Vertex_2r &p);

private:
    SplayTree<Arrangement_Bundle> btree_;

};

//=============================================================================
// Interface: Bundle_List
//=============================================================================

class Bundle_List {
public:
    //Bundle_List();
    void Remove(std::list<Arrangement_Bundle>::iterator where);
    void Insert(std::list<Arrangement_Bundle>::iterator where, Arrangement_Bundle& to_insert);
    void LocatePoint(Arrangement_Vertex_2r &p, Arrangement_Bundle &top, Arrangement_Bundle &bottom);
    void SplitBundles(Arrangement_Vertex_2r &p);
    std::list<Arrangement_Bundle>::iterator FindIndex(const Segment_2r_colored& target_segment) ;


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

inline bool operator<(const Arrangement_Bundle &lhs, const Arrangement_Bundle &rhs){
    // Only works for same-colored bundles!
    return lhs.getTop() < rhs.getBot();
}
inline bool operator>(const Arrangement_Bundle &lhs, const Arrangement_Bundle &rhs){
    // Only works for same-colored bundles!
    return lhs.getBot() > rhs.getTop();
}
inline bool operator==(const Arrangement_Bundle &lhs, const Arrangement_Bundle &rhs){
    return lhs.getRoot() == rhs.getRoot(); // If weird things happen, it might be because this method doesn't check for an element-by-element equivalence between two bundles.
}
inline bool operator!=(const Arrangement_Bundle &lhs, const Arrangement_Bundle &rhs){
    return !(lhs == rhs);
}

} // namespace DDAD

#endif // GE_ARRANGEMENT_H
