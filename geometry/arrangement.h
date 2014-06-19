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

class Bundle;

typedef std::shared_ptr<Bundle> SharedBundle;
typedef std::shared_ptr<Segment_2r_colored> SharedSegment;

//=============================================================================
// Interface: ArrangementVertex_2r
//=============================================================================

class ArrangementVertex_2r {
public:
    ArrangementVertex_2r();
    ArrangementVertex_2r(SharedPoint_2r& pt,
                         SharedPoint_2r& other_point, bool  is_red);

    const bool is_red() { return color_; }
    SharedPoint_2r get_other_point() const { return other_point_; }
    SharedPoint_2r get_point() const { return point_; }
    const rational& get_x() { return point_->x(); }
    const rational& get_y() { return point_->y(); }

private:
    bool color_;
    SharedPoint_2r other_point_;
    SharedPoint_2r point_;
};

//=============================================================================
// Interface: Bundle
//=============================================================================

class Bundle {
public:
    Bundle();
    Bundle(SplayTree<Segment_2r_colored>& rhs);

    //Accessors
    SharedBundle get_next_bundle() { return next_bundle_; }
    SharedBundle get_prev_bundle() { return prev_bundle_; }
    SharedSegment get_top_seg() { return top_segment_; }
    SharedSegment get_bot_seg() { return bottom_segment_; }
    SplayTree<Segment_2r_colored>* get_tree() { return &tree_; }
    SharedBundle get_sptr() { return std::make_shared<Bundle>(this); }
    void set_next_bundle(SharedBundle new_next) { next_bundle_ = new_next; }
    void set_prev_bundle(SharedBundle new_prev) { prev_bundle_ = new_prev; }

    //Class methods
    void Insert(SharedSegment new_segment);
    void Remove(SharedSegment old_segment);
    SharedBundle Split(SharedSegment split_here);
    SharedBundle Merge(SharedBundle to_merge);

private:
    //pointers to next and previous bundles in linked list
    SharedBundle next_bundle_;
    SharedBundle prev_bundle_;
    //the bundle itself - tree of segments
    SplayTree<Segment_2r_colored> tree_;
    //pointers to the top and bottom of the bundle
    SharedSegment top_segment_;
    SharedSegment bottom_segment_;
};

//=============================================================================
// Interface: BundleTree
//=============================================================================

class BundleTree {
public:
    //For a segment input_segment, determine the bundles immediately above
    //and below in the arrangement.  If within a bundle, both above_neighbor
    //and below_neighbor should be that bundle.
    void LocateVertex(ArrangementVertex_2r &input_vertex, SharedBundle above_neighbor,
                     SharedBundle below_neighbor);
    void AddBundle(SharedBundle add_this);
    void RemoveBundle(SharedBundle remove_this);
    void SplitBundleAtVertex(ArrangementVertex_2r & split_here);
private:
    SplayTree<SharedBundle> bundle_tree_;
};

//=============================================================================
// Interface: Bundle_List
//=============================================================================

class BundleList{
public:
    void InsertBundle(SharedBundle insert_this, SharedBundle after_this);
    void MergeBundles(SharedBundle merge_this, SharedBundle with_this);

private:
    SharedBundle front_;
    SharedBundle back_;
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

    const std::list<ArrangementVertex_2r>& get_vertices() const;
    const std::list<Segment_2r_colored>& get_segments() const;

private:
    std::list<ArrangementVertex_2r> vertices_;
    std::list<Segment_2r_colored> segments_;
    Point_2r floater_;
    bool current_color_;
};

int CountIntersections(const Arrangement_2r& A,
                       Visual::IGeometryObserver* observer = nullptr);

} // namespace DDAD

#endif // GE_ARRANGEMENT_H
