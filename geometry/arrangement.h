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

enum RelativePosition{
    ABOVE = 1,
    ENDING = 0,
    BELOW = -1
};

class Bundle;
typedef std::shared_ptr<Bundle> SharedBundle;


class BundleTree;
class BundleList;

//=============================================================================
// Interface: ArrangementVertex_2r
//=============================================================================

class ArrangementVertex_2r {
public:
    ArrangementVertex_2r();
    ArrangementVertex_2r(SharedPoint_2r& pt,
                         SharedPoint_2r& other_point, bool  is_red,
                         SharedSegment& my_segment);

    const bool is_red() { return color_; }
    SharedPoint_2r get_other_point() const { return other_point_; }
    SharedPoint_2r get_point() const { return point_; }
    const rational& get_x() { return point_->x(); }
    const rational& get_y() { return point_->y(); }
    SharedSegment get_segment() const { return my_segment_; }

private:
    bool color_;
    SharedPoint_2r other_point_;
    SharedPoint_2r point_;
    SharedSegment my_segment_;
};

//=============================================================================
// Interface: Bundle
//=============================================================================

class Bundle : public SplayTree<SharedSegment>{
    friend class BundleTree;
    friend class BundleList;
public:
    // pointers to next and previous bundles in linked list
    SharedBundle next_bundle_;
    SharedBundle prev_bundle_;
    // pointers to the top and bottom of the bundle
    SharedSegment top_segment_;
    SharedSegment bottom_segment_;
    // Position relative to the last vertex accessed
    RelativePosition rel_position_;

    Bundle();
    Bundle(SplayTree<SharedSegment>& rhs);
    Bundle(BinaryNode<SharedSegment>* new_root);
    ~Bundle();

    //Class methods
    bool get_color() { return bottom_segment_->get_color(); }
    void Splay(const SharedSegment& x, BinaryNode<SharedSegment>* t);
    void Splay (const Point_2r &x, BinaryNode<SharedSegment> *t);
    void Insert(const SharedSegment& new_segment);
    bool Contains(const ArrangementVertex_2r& test_point);
    RelativePosition SetRelativePosition(ArrangementVertex_2r& test_point);
    void Merge(SharedBundle to_merge);
    SharedBundle Split(Point_2r& split_here,
                       SharedBundle& my_sptr);

};

//=============================================================================
// Interface: BundleTree
//=============================================================================

class BundleTree : public SplayTree<SharedBundle>{
public:
    void Splay (const SharedBundle& x, BinaryNode<SharedBundle>* t);
    void Splay (const Point_2r& x, BinaryNode<SharedBundle>* t);
    void Insert(const SharedBundle& new_bundle);
};

//=============================================================================
// Interface: Bundle_List
//=============================================================================

class BundleList{
public:
    SharedBundle bottom_;
    SharedBundle top_;
    void GenerateSentinels(std::list<ArrangementVertex_2r> L,
                           BundleTree& bdt);
    void InsertBundle(SharedBundle insert_this, SharedBundle after_this);
    void RemoveBundle(SharedBundle remove_this);
    void LocateVertex(ArrangementVertex_2r &input_vertex,
                      BundleTree& bdt,
                      SharedBundle& red_above,
                      SharedBundle& red_below,
                      SharedBundle& blue_above,
                      SharedBundle& blue_below);
    SharedBundle SplitBundleAtVertex(SharedBundle split_bundle,
                                     ArrangementVertex_2r &here);
    void SplitBundlesContaining(ArrangementVertex_2r& input_vertex,
                                BundleTree& bdt,
                                SharedBundle& top,
                                SharedBundle& bot);
    int SortPortion(ArrangementVertex_2r& v, SharedBundle& begin,
                    SharedBundle& end);
    void SwapAdjacentBundles(SharedBundle& left, SharedBundle& right);
    void InsertLeftEndpoint(ArrangementVertex_2r& input_vertex,
                            BundleTree& bdt);
    void RemoveRightEndpoint(ArrangementVertex_2r& input_vertex,
                             BundleTree& bdt);
    void MergeOrderedBundles(BundleTree& bdt);
    void SwapBundles(SharedBundle &a, SharedBundle &b);
    void PrintState(SharedBundle& start, SharedBundle& end);

};

//=============================================================================
// Interface: Arrangement_2r
//=============================================================================

class Arrangement_2r : public Visual::Geometry {
public:
    ~Arrangement_2r();

    void AddSegment(Point_2r& v, Point_2r& w, bool color);
    void EndSegment(Point_2r& v);
    void PushPoint(Point_2r& v, bool color);
    void PushPoint(SharedPoint_2r v, bool color);
    void PopPoint();

    const std::list<ArrangementVertex_2r>& get_vertices() const
    {
        return vertices_;
    }
    const std::list<SharedSegment>& get_segments() const
    {
        return segments_;
    }

private:
    std::list<ArrangementVertex_2r> vertices_;
    std::list<SharedSegment> segments_;
    Point_2r floater_;
    bool current_color_;
};

int CountIntersections(const Arrangement_2r& A,
                       Visual::IGeometryObserver* observer = nullptr);

} // namespace DDAD



#endif // GE_ARRANGEMENT_H
