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
    ABOVE = 1,
    ENDING = 0,
    BELOW = -1
};

class Bundle;
typedef std::shared_ptr<Bundle> SharedBundle;
typedef std::shared_ptr<Segment_2r_colored> SharedSegment;

class BundleTree;
class BundleList;

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
friend class BundleTree;
friend class BundleList;
public:
    Bundle();
    Bundle(SplayTree<Segment_2r_colored>& rhs);

    //Accessors
    const SharedBundle get_next_bundle() const { return next_bundle_; }
    const SharedBundle get_prev_bundle() const { return prev_bundle_; }
    SharedBundle get_prev_bundle() { return prev_bundle_; }
    SharedBundle get_next_bundle() { return next_bundle_; }
    const SharedSegment get_top_seg() const { return top_segment_; }
    const SharedSegment get_bot_seg() const { return bottom_segment_; }
    const RelativePosition get_rel_position() const { return rel_position_; }
    const BinaryNode<Segment_2r_colored>* get_root() const
    {
        return tree_.getRoot();
    }
    SplayTree<Segment_2r_colored>* get_tree()  { return &tree_; }
    bool get_color() { return top_segment_->get_color(); }
    void set_next_bundle(SharedBundle new_next) { next_bundle_ = new_next; }
    void set_prev_bundle(SharedBundle new_prev) { prev_bundle_ = new_prev; }

    //Class methods
    void Insert(SharedSegment new_segment);
    void Remove(SharedSegment old_segment);
    bool Contains(ArrangementVertex_2r& test_point);

    RelativePosition SetRelativePosition(ArrangementVertex_2r& test_point);
    int CountSegments();
    void Merge(SharedBundle to_merge);
    SharedBundle Split(SharedSegment split_here);
    SharedBundle Split(Point_2r& split_here);

private:
    //pointers to next and previous bundles in linked list
    SharedBundle next_bundle_;
    SharedBundle prev_bundle_;
    //the bundle itself - tree of segments
    SplayTree<Segment_2r_colored> tree_;
    //pointers to the top and bottom of the bundle
    SharedSegment top_segment_;
    SharedSegment bottom_segment_;
    RelativePosition rel_position_;

};

//=============================================================================
// Interface: BundleTree
//=============================================================================

class BundleTree {
public:
    //For a segment input_segment, determine the bundles immediately above
    //and below in the arrangement.  If within a bundle, both above_neighbor
    //and below_neighbor should be that bundle.
    SharedBundle get_root()
    {
        if(bundle_tree_.isEmpty()) return nullptr;
        else return bundle_tree_.getRoot()->getElement();
    }
    bool is_empty() { return bundle_tree_.isEmpty(); }
    SplayTree<SharedBundle> get_tree() { return bundle_tree_; }
    void LocateVertex(ArrangementVertex_2r &input_vertex,
                      SharedBundle &above_neighbor,
                      SharedBundle &below_neighbor);
    void InsertBundle(SharedBundle add_this);
    void RemoveBundle(SharedBundle remove_this);
    int Size();
    SharedBundle SplitBundleAtVertex(ArrangementVertex_2r & split_here);
    void Find(ArrangementVertex_2r& input_vertex);
    void MergeBundles(SharedBundle above, SharedBundle below);
private:
    SplayTree<SharedBundle> bundle_tree_;
};

//=============================================================================
// Interface: Bundle_List
//=============================================================================

class BundleList{
public:
    void set_bottom(SharedBundle new_bottom) { bottom_ = new_bottom; }
    void set_top(SharedBundle new_top) { top_ = new_top; }
    SharedBundle get_bottom() { return bottom_; }
    SharedBundle get_top() { return top_; }
    void GenerateSentinels(std::list<ArrangementVertex_2r> L,
                           BundleTree& bdt);
    void InsertBundle(SharedBundle insert_this, SharedBundle after_this);
    void RemoveBundle(SharedBundle remove_this);
    SharedBundle LocateVertex(ArrangementVertex_2r &input_vertex,
                              BundleTree& bdt);
    SharedBundle SplitBundleAtVertex(SharedBundle split_bundle,
                             ArrangementVertex_2r &here);
    int SortPortion(SharedBundle &begin, SharedBundle &end,
                    ArrangementVertex_2r v);
    void SwapAdjacentBundles(SharedBundle& left, SharedBundle& right);
    void SwapBundles(SharedBundle &a, SharedBundle &b);

private:
    SharedBundle bottom_;
    SharedBundle top_;
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

inline bool operator<(const Bundle &lhs, const Bundle &rhs){
    return Predicate::AIsRightOfB(lhs.get_top_seg()->p(),
                                  rhs.get_bot_seg()->support());
    // Only works for same-colored bundles!
//    return lhs.get_top_seg() < rhs.get_bot_seg();
}
inline bool operator>(const Bundle &lhs, const Bundle &rhs){
    return Predicate::AIsLeftOfB(lhs.get_bot_seg()->p(),
                                 rhs.get_top_seg()->support());
    // Only works for same-colored bundles!
//    return lhs.get_bot_seg() > rhs.get_top_seg();
}
inline bool operator==(const Bundle &lhs, const Bundle &rhs){
    return lhs.get_root() == rhs.get_root();
    // If weird things happen, it might be because this method doesn't check for
    // an element-by-element equivalence between two bundles.
}
inline bool operator!=(const Bundle &lhs, const Bundle &rhs){
    return !(lhs == rhs);
}
inline bool operator<(const SharedBundle &lhs, const SharedBundle &rhs){
    return Predicate::AIsRightOfB(lhs->get_top_seg()->p(),
                                  rhs->get_bot_seg()->support());
    // Only works for same-colored bundles!
}
inline bool operator>(const SharedBundle &lhs, const SharedBundle &rhs){
    return Predicate::AIsLeftOfB(lhs->get_bot_seg()->p(),
                                 rhs->get_top_seg()->support());
    // Only works for same-colored bundles!
}
//inline bool operator==(const SharedBundle &lhs, const SharedBundle &rhs){
//    return lhs->get_root() == rhs->get_root();
//}
//inline bool operator!=(const SharedBundle &lhs, const SharedBundle &rhs){
//    return !(lhs == rhs);
//}
inline bool operator<(const Point_2r& lhs, const SharedBundle& rhs)
{
    return Predicate::AIsRightOfB(lhs, rhs->get_bot_seg()->support());
}
inline bool operator>(const Point_2r& lhs, const SharedBundle& rhs)
{
    return Predicate::AIsLeftOfB(lhs, rhs->get_bot_seg()->support());
}
inline bool operator==(const Point_2r& lhs, const SharedBundle& rhs)
{
    return (Predicate::AIsRightOrAheadOfB(lhs,
                                          rhs->get_top_seg()->support_ray())
            && Predicate::AIsLeftOrAheadOfB(lhs,
                                            rhs->get_bot_seg()->support_ray()));
}
inline bool operator<=(const Point_2r& lhs, const SharedBundle& rhs)
{
    return (lhs < rhs || lhs == rhs);
}
inline bool operator>=(const Point_2r& lhs, const SharedBundle& rhs)
{
    return (lhs > rhs || lhs == rhs);
}
} // namespace DDAD



#endif // GE_ARRANGEMENT_H
