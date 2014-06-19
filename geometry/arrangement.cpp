#include "arithmetic.h"
#include "line.h"
#include "intersection.h"
#include "arrangement.h"


namespace DDAD {

/*
int CountIntersections(const Arrangement_2r &A, Visual::IGeometryObserver *observer){
    // Do this on A
//# Invariants for the algorithm:
//#
//#	- All intersections whose witnesses are left of the sweepline have been reported
//#	- The order of segments along the sweepline is consistent with pushing all intersections as far right as possible
//#
//# Events:
//#	- Sweep line reaches an endpoint
//#
//# Preconditions:
//#	- red/red and blue/blue intersections do not exist (such a coloring does exist)
//#
//# Output:
//#	- The number of intersections that occur
//#
//# Input:
//#	- set of segments, each colored red or blue
//#
//#
    int crossings = 0;
    std::list<ArrangementVertex_2r> L = A.get_vertices();
    //vertex comparator
    struct compare{
                    bool operator()(ArrangementVertex_2r a, ArrangementVertex_2r b){
                        if(a.get_x() < b.get_x()){return true;}
                        if(a.get_x() > b.get_x()){return false;}
                        return a.get_y() < b.get_y();
                    }
                };
    //sort vertices lexicographically
    L.sort(compare());

    // Initialize data structures
    BundleTree bdt = BundleTree();
    BundleList bdl = BundleList();
    Bundle top_red;
    Bundle bot_red;
    Bundle top_blue;
    Bundle bot_blue;
    RelativePosition top_red_position;
    RelativePosition bot_red_position;
    RelativePosition top_blue_position;
    RelativePosition bot_blue_position;

    //for each event p in L do:

    for(std::list<ArrangementVertex_2r>::iterator ii = L.begin(); ii != L.end(); ii ++){

        //	split(p, brundletree, red)
        //	split(p, brundlelist, blue)
        Segment_2r_colored current_segment = Segment_2r_colored(ii->get_point(), ii->get_other_point(), ii->is_red());
        // Check on the order of the bundles around the current point

        //	if bundles r and b in wrong positions:
        //		countIntersections(rxb)
        //		swap(r,b)
        // repair brundletree by merge
        // repair brundlelist
        // Insert any new bundles
        if(*(ii->get_point()) < *(ii->get_other_point())){ // Current point is a left-endpoint
            // Search for the point in the linked list
            Bundle new_bundle = Bundle();
            new_bundle.Insert(std::make_shared<Segment_2r_colored>(current_segment));
//            std::list<Bundle>::iterator pos_in_list = bdl.FindIndex(current_segment);
//            bdl.Insert(pos_in_list, new_bundle);
//            bdt.insert(new_bundle);
        }
        else{ // Current point is a right-endpoint, pop the segment
            // First from the bundle tree
            Bundle tmp_bundle = Bundle();
            tmp_bundle.Insert(current_segment);
            bdt.find(tmp_bundle);
            bdt.getRoot()->element.remove(current_segment);
            if(bdt.getRoot()->element.isEmpty()) bdt.remove(bdt.getRoot()->element);
            // Then from the list
            std::list<Bundle>::iterator bundle_it = bdl.FindIndex(current_segment);
            bundle_it->remove(current_segment);
            if(bundle_it->isEmpty()) bdl.Remove(bundle_it);
        }

        crossings++;
    }


    return crossings;
}
*/

//=============================================================================
// Implementation: ArrangementVertex_2r
//=============================================================================

ArrangementVertex_2r::ArrangementVertex_2r(SharedPoint_2r &pt, SharedPoint_2r &other_point, bool is_red){
    color_ = is_red;
    point_ = pt;
    other_point_ = other_point;
}


//=============================================================================
// Implementation: Bundle
//=============================================================================

Bundle::Bundle(){
    top_segment_ = nullptr;
    bottom_segment_ = nullptr;
    next_bundle_ = nullptr;
    prev_bundle_ = nullptr;
    tree_ = SplayTree<Segment_2r_colored>();
}

Bundle::Bundle(SplayTree<Segment_2r_colored>& the_tree){
    the_tree.findMax();
    top_segment_ = std::make_shared<Segment_2r_colored>(the_tree.getRoot()->getElement());
    the_tree.findMin();
    bottom_segment_ = std::make_shared<Segment_2r_colored>(the_tree.getRoot()->getElement());
    next_bundle_ = nullptr;
    prev_bundle_ = nullptr;
    tree_ = the_tree;
}

void Bundle::Insert(SharedSegment new_segment){
    // Make sure segments are inserted in right-left order
    if(new_segment->q() < new_segment->p()){
        Point_2r new_p = new_segment->q();
        Point_2r new_q = new_segment->p();
        bool new_color = new_segment->get_color();
        tree_.insert(Segment_2r_colored(new_p, new_q, new_color));
    }
    else{
        tree_.insert(*new_segment);
    }
    // Make sure top_segment and bottom_segment have been set
    if(top_segment_ == nullptr && bottom_segment_ == nullptr){
        top_segment_ = bottom_segment_ = new_segment;
    }
    else{
        if(*top_segment_ < *new_segment) top_segment_ = new_segment;
        if(*new_segment < *bottom_segment_) bottom_segment_ = new_segment;
    }
}

void Bundle::Remove(SharedSegment old_segment){
    tree_.remove(*old_segment);
    // Code to delete the bundle if it is empty should exist in both list and tree
}

SharedBundle Bundle::Split(SharedSegment split_here){
    SplayTree<Segment_2r_colored> R = tree_.splitTree(*split_here);
    SharedBundle new_bundle = std::make_shared<Bundle>(R);

    // Set next_bundle and prev_bundle pointers
    new_bundle->set_prev_bundle(get_sptr());
    new_bundle->set_next_bundle(next_bundle_);
    if(next_bundle_ != nullptr) next_bundle_->set_prev_bundle(new_bundle);
    next_bundle_ = new_bundle;

}

SharedBundle Bundle::Merge(SharedBundle to_merge){
    // Merges to_merge into the current bundle
    if(to_merge->get_next_bundle() != nullptr){
        to_merge->get_next_bundle()->set_prev_bundle(get_sptr());
    }
    if(to_merge->get_prev_bundle() != nullptr){
        to_merge->get_prev_bundle()->set_next_bundle(get_sptr());
    }
    tree_.mergeTree(to_merge->get_tree());
}

bool Bundle::Contains(ArrangementVertex_2r &test_point){
    Point_2r the_point = *(test_point.get_point());
    return (Predicate::AIsRightOfB(the_point, top_segment_->support()) &&
            Predicate::AIsLeftOfB(the_point, bottom_segment_->support()));
}


//=============================================================================
// Implementation: BundleTree
//=============================================================================

void BundleTree::Find(ArrangementVertex_2r& input_vertex){
    Segment_2r_colored tmp_segment = Segment_2r_colored(input_vertex.get_point(), input_vertex.get_other_point(), input_vertex.is_red());
    SharedBundle tmp_bundle = std::make_shared<Bundle>();
    tmp_bundle->Insert(std::make_shared<Segment_2r_colored>(tmp_segment));
    bundle_tree_.find(tmp_bundle);
}

void BundleTree::LocateVertex(ArrangementVertex_2r &input_vertex,
                              SharedBundle above_neighbor,
                              SharedBundle below_neighbor)
{
    // For an empty tree, our job is easy
    if(bundle_tree_.isEmpty()){
        above_neighbor = nullptr;
        below_neighbor = nullptr;
        return;
    }
    Find(input_vertex);
    Segment_2r_colored tmp_segment = Segment_2r_colored(input_vertex.get_point(), input_vertex.get_other_point(), input_vertex.is_red());
    SharedBundle tmp_bundle = std::make_shared<Bundle>();
    tmp_bundle->Insert(std::make_shared<Segment_2r_colored>(tmp_segment));

    // Now either the uppermost bundle below or containing the input vertex is now at the root, or no such bundles exist and the lowest bundle in the tree is now at the root
    if(*(bundle_tree_.getRoot()->getElement()) > *(tmp_bundle)){
        // input_vertex is below the entire tree
        above_neighbor = bundle_tree_.getRoot()->getElement();
        below_neighbor = nullptr;
    }
    else{
        // search down the tree for the lowest bundle above input_vertex
        below_neighbor = bundle_tree_.getRoot()->getElement();
        BinaryNode<SharedBundle>* tmp_ptr = bundle_tree_.getRoot();
        if(tmp_ptr->right == nullptr) above_neighbor = nullptr;
        else{
            while(tmp_ptr->left != nullptr) tmp_ptr = tmp_ptr->left;
            above_neighbor = tmp_ptr->getElement();
        }
    }
}

void BundleTree::AddBundle(SharedBundle add_this)
{
    bundle_tree_.insert(add_this);
}

void BundleTree::RemoveBundle(SharedBundle remove_this)
{
    bundle_tree_.remove(remove_this);
}

void BundleTree::SplitBundleAtVertex(ArrangementVertex_2r &split_here)
{
    Find(split_here);  // Rotate the appropriate bundle to the root
    if(bundle_tree_.getRoot()->getElement()->Contains(split_here)){
        // Create a new bundle from the split
        SharedSegment tmp_segment = std::make_shared<Segment_2r_colored>(split_here.get_point(), split_here.get_other_point(), split_here.is_red());
        SharedBundle new_bundle =
                bundle_tree_.getRoot()->element->Split(tmp_segment);
    }
}

void BundleList::InsertBundle(SharedBundle insert_this, SharedBundle after_this){
    insert_this->set_next_bundle(after_this->get_next_bundle());
    insert_this->set_prev_bundle(after_this->get_sptr());
    after_this->set_next_bundle(insert_this->get_sptr());
    insert_this->get_next_bundle()->set_prev_bundle(insert_this->get_sptr());
}

void BundleList::MergeBundles(SharedBundle merge_this, SharedBundle with_this){

}




//=============================================================================
// Implementation: Arrangement_2r
//=============================================================================

Arrangement_2r::~Arrangement_2r() {
    for(auto i = begin(segments_); i != end(segments_); ++i) {
        SigPopVisualSegment_2r(*i);
    }
    for(auto i = begin(vertices_); i != end(vertices_); ++i) {
        SigPopVisualPoint_2r(*(i->get_point()));
    }
}

void Arrangement_2r::AddSegment(Point_2r& v, Point_2r& w, bool color){
//    LOG(INFO) << "in AddSegment";
    segments_.push_front(Segment_2r_colored(v, w, color));
    vertices_.push_front(ArrangementVertex_2r(std::make_shared<Point_2r>(v), std::make_shared<Point_2r>(w), color));
    vertices_.push_front(ArrangementVertex_2r(std::make_shared<Point_2r>(w), std::make_shared<Point_2r>(v), color));
//    LOG(INFO) << "Pushed a segment to the list";
    //Create visual point and segment
    Visual::Material vMat;
    if(color) {
        vMat.set_ambient(Visual::Color(255, 0, 0, 255));
    }
    else {
        vMat.set_ambient(Visual::Color(0, 0, 255, 255));
    }
    Visual::Point vPoint(vMat);
    Visual::Segment vSeg(vMat);

    SigRegisterPoint_2r(v);
    SigRegisterPoint_2r(w);
    SigPushVisualPoint_2r(v,vPoint);
    SigPushVisualPoint_2r(w,vPoint);
    Segment_2r edge(std::make_shared<Point_2r>(v), std::make_shared<Point_2r>(w));
    SigPushVisualSegment_2r(edge, vSeg, 10);
//    LOG(INFO) << "Pushed a segment";
}

void Arrangement_2r::EndSegment(Point_2r &v){
    AddSegment(floater_, v, current_color_);
}

void Arrangement_2r::PushPoint(Point_2r &v, bool color){
    PushPoint(std::make_shared<Point_2r>(v), color);
}

void Arrangement_2r::PushPoint(SharedPoint_2r v, bool color){
//    LOG(INFO) << "Pushed point";
    floater_ = *v;
    current_color_ = color;

    //Create visual point
    Visual::Material vMat;
    if(color) {
        vMat.set_ambient(Visual::Color(255, 0, 0, 255));
    }
    else {
        vMat.set_ambient(Visual::Color(0, 0, 255, 255));
    }
    Visual::Point vPoint(vMat);

    //Push the point
    SigRegisterPoint_2r(floater_);
    SigPushVisualPoint_2r(floater_,vPoint);
}

void Arrangement_2r::PopPoint(){
    SigPopVisualPoint_2r(floater_);
}

const std::list<Segment_2r_colored>& Arrangement_2r::get_segments() const{
    return segments_;
}

const std::list<ArrangementVertex_2r>& Arrangement_2r::get_vertices() const{
    return vertices_;
}

} // Namespace DDAD
