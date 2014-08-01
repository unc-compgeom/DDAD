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

class BundleTree;
class BundleList;

//=============================================================================
// Interface: ArrangementVertex_2r
//=============================================================================
/**
 * @brief Colored vertex in an Arrangement_2r.  It knows its coordinates (in the mpir rational class) and the segment containing it.
 */
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
/**
 * @brief Bundle class implemented as a SplayTree of shared_ptrs to segments.  Overridden Splay method requires that segments in the bundle satisfy the invariant "there is a vertical sweepline intersecting all segments in the bundle", so this invariant is very important to maintain.
 */
class Bundle : public SplayTree<SharedSegment>{
    friend class BundleTree;
    friend class BundleList;
public:
    // pointers to next and previous bundles in linked list
    Bundle* next_bundle_;
    Bundle* prev_bundle_;
    // pointers to the top and bottom of the bundle
    SharedSegment top_segment_;
    SharedSegment bottom_segment_;

    /**
     * @brief Base constructor/destructor and copy constructors
     */
    Bundle();
    Bundle(SplayTree<SharedSegment>& rhs);
    Bundle(BinaryNode<SharedSegment>* new_root);
    Bundle(SharedSegment& new_segment);
    ~Bundle();

    //Class methods
    bool get_color() { return bottom_segment_->get_color(); }

    /**
     * @brief Internal method to perform a top-down Splay in log(n) time.
     *  The last accessed node becomes the new root. If x is not
     *  contained in the tree, then the root of the resulting tree is the
     *  greatest element < x or, if no such element exists in the tree, the
     *  least element > x.
     * @param x : The target item to Splay around
     * @param t : the root of the subtree to Splay
     */
    void Splay( SharedSegment x, BinaryNode<SharedSegment>* t) override;
    void Splay (Point_2r &x, BinaryNode<SharedSegment> *t);

    /**
     * @brief Insert a segment into the bundle (duplicates not allowed) in
     *  log(n) time.
     * There must be a sweepline intersecting the new segment and all other
     *  segments in the bundle
     */
    void Insert(const SharedSegment& new_segment);

    /**
     * @brief Merge a bundle into the current bundle in log(n) time.
     * @param to_merge: Must be strictly above the current bundle
     * (to_merge->bottom_segment->IsAbove(this->top_segment_) must be true)
     * This will splay the top segment of the current bundle to the root
     */
    void Merge(Bundle* to_merge);

    /**
     * @brief Test to check if a test_point lies between top_segment_ and
     *  bottom_segment.
     * @param test_point: must lie on a sweepline intersecting top_segment
     *  and bottom_segment (does not check to make sure of this)
     * @return: true if test_point is contained in the bundle, false otherwise
     */
    bool Contains(const ArrangementVertex_2r& test_point);

    /**
     * @brief Test to check if a segment is already in the bundle
     */
    bool ContainsValue(const SharedSegment &x);

    /**
     * @brief Remove a segment from the bundle.
     * @param x: If x is a segment in the bundle, then it will be removed
     *  from the tree.  Otherwise, nothing will happen
     * @return: true if x was successfully removed, and false if x not found
     */
    bool Remove(const SharedSegment &x);

    /**
     * @brief Position of the bundle relative to an input vertex. One of:
     * ABOVE (+1): input vertex lies to the right of bottom_segment_
     * BELOW (-1): input vertex lies to the left of top_segment_
     * ENDING (0): input vertex is one of the endpoints of top_segment_ or
     *  bottom_segment
     *
     * rel_position_ should only be set after any bundles containing the
     *  input_vertex have already been split, since there is not an enum
     *  corresponding with an input_vertex contained within the bundle.
     */
    RelativePosition rel_position_;
    RelativePosition SetRelativePosition(ArrangementVertex_2r& test_point);

    /**
     * @brief Split this bundle at an input vertex into two subtrees (L)
     *  and (R) such that !(x->IsAbove(r)) for all x in L and r in R.
     * @param split_here: any vertex which intersects a vertcal sweepline
     *  which intersects all segments in the bundle
     * @return: subtree R as a new Bundle.  Subtree L will remain in *this
     */
    Bundle* Split(Point_2r& split_here);

};

//=============================================================================
// Interface: BundleTree
//=============================================================================

class BundleTree : public SplayTree<Bundle*>{
public:
    /**
     * @brief Internal method to perform a top-down Splay in log(n) time.
     *  The last accessed node becomes the new root. If x is not
     *  contained in the tree, then the root of the resulting tree is the
     *  greatest element < x or, if no such element exists in the tree, the
     *  least element > x.
     * @param x : The target item to Splay around
     * @param t : the root of the subtree to Splay
     */
    void Splay ( Bundle* x, BinaryNode<Bundle*>* t) override;
    void Splay ( Point_2r& x, BinaryNode<Bundle*>* t);

    /**
     * @brief Insert a new bundle into the tree in log(n) time.
     * @param new_bundle: There must be a vertical sweepline that intersects
     *  new_bundle (all segments in new_bundle) and all segments in other
     *  bundles already in the tree.
     */
    void Insert(Bundle* new_bundle);

    /**
     * @brief Remove a bundle from the BundleTree.
     *  (@TODO: this method does not properly clean up the removed bundle)
     */
    bool Remove(Bundle* remove_this);

    /**
     * @brief Test whether a test bundle x is one of the bundles in the
     *  BundleTree (log(n) search).  Checks for pointer equivalence rather than
     *  element-by-element equivalence, so will not remove a copy of a bundle
     *  in the tree.  This method DOES NOT repair the the linked list after
     *  removal (BundleList::Remove(remove_this) must also be called afterwards)
     * @return: true if remove_this was removed from the list, false if
     *  remove_this was not found in the Bundletree
     */
    bool ContainsValue(Bundle * const &x);

    /**
     * @brief Split any bundle containing input_vertex.  See Bundle::Split for
     *  the conditions satisfied by each of the resulting subtrees. After this,
     *  the linked-list will reflect the split, but bundles in the list may no
     *  longer properly alternate colors.
     * @param input_vertex: any vertex which intersects a vertical sweepline
     *  that intersects all segments currently contained in bundles in the tree
     */
    void SplitAtVertex(const ArrangementVertex_2r input_vertex);
};

//=============================================================================
// Interface: Bundle_List
//=============================================================================

class BundleList{
public:
    Bundle* bottom_;
    Bundle* top_;
    void GenerateSentinels(std::list<ArrangementVertex_2r> L, BundleTree& bdt);
    void Insert(Bundle* insert_this, Bundle* after_this);
    void Remove(Bundle* remove_this);
    void LocateVertex(ArrangementVertex_2r& input_vertex,
                      BundleTree& bdt,
                      Bundle *&red_above,
                      Bundle *&red_below,
                      Bundle *&blue_above,
                      Bundle *&blue_below);
    Bundle* SplitBundleAtVertex(Bundle* split_bundle,
                                     ArrangementVertex_2r& here);
    void SplitBundlesContaining(ArrangementVertex_2r& input_vertex,
                                Bundle* top,
                                Bundle* bot);
    int SortPortion(ArrangementVertex_2r& v, Bundle* begin, Bundle* end);
    void SwapAdjacentBundles(Bundle* left, Bundle* right);
    void InsertLeftEndpoint(ArrangementVertex_2r& input_vertex,
                            BundleTree& bdt);
    void RemoveRightEndpoint(ArrangementVertex_2r& input_vertex,
                             BundleTree& bdt);
    void MergeOrderedBundles(BundleTree& bdt);
    void PrintState(Bundle* start, Bundle* end);
    bool CheckInvariants();
    bool CheckCorrectOrder() const;
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

/**
 * @brief Degree-Driven algorithm to count intersections in a given arrangement
 *  between red and blue segments.
 * @param A
 * @param observer
 * @return
 */
int CountIntersections(const Arrangement_2r& A,
                       Visual::IGeometryObserver* observer = nullptr);

} // namespace DDAD



#endif // GE_ARRANGEMENT_H
