#include "arithmetic.h"
#include "line.h"
#include "intersection.h"
#include "arrangement.h"


namespace DDAD {

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


//=============================================================================
// Implementation: BundleTree
//=============================================================================

void BundleTree::LocateVertex(ArrangementVertex_2r &input_vertex,
                              SharedBundle above_neighbor,
                              SharedBundle below_neighbor)
{
    Segment_2r_colored test_segment = Segment_2r_colored(input_vertex.get_point(), input_vertex.get_other_point(), input_vertex.is_red());
    bundle_tree_;
}


void BundleTree::LocatePoint(ArrangementVertex_2r &p, Bundle &top, Bundle &bot){
    // Splits bundles in the bundle tree at current vertex, then locates the current vertex among the red bundles in the bundle tree.
    // top = lowest bundle above or ending at p
    // bot = highest bundle below or ending at p
    if(root == nullptr) return;
    splitBundles(p);
    Bundle tmpBundle = Bundle();
    tmpBundle.insert(Segment_2r_colored(p.getPoint(), p.getPoint(), p.get_color()));
    find(tmpBundle);  // Splay to p
    if(Predicate::AIsRightOfB(p.position(), root->getElement().getBot()->support())){
        // p is below the entire tree
        top = root->getElement();
        bot = Bundle();
        return;
    }
    else{
        bot = root->getElement();
        if(root->right == nullptr){
            top = Bundle();
            return;
        }
        BinaryNode<Bundle>* temp_node = root->right;
        while(temp_node->left != nullptr) temp_node = temp_node->left;
        top = temp_node->getElement();
    }

}

void BundleTree::splitBundles(ArrangementVertex_2r &p){

    Bundle tmpBundle = Bundle();
    tmpBundle.insert(Segment_2r_colored(p.getPoint(), p.getPoint(), p.get_color()));
    find(tmpBundle);
    Segment_2r_colored topSeg = *(root->getElement().getTop());    //top segment of bundle below or containing p
    Segment_2r_colored botSeg;   //bottom segment of bundle above or containing p

    if(Predicate::AIsRightOfB(p.position(),topSeg.support())){
        //p is in the bottom bundle
        Bundle R = Bundle(root->getElement().splitTree(Segment_2r_colored(p.getPoint(), p.getOtherPoint(), p.get_color())));
        BinaryNode<Bundle>* new_node = new BinaryNode<Bundle>(R, nullptr, root->right);
        root->right = new_node;
        return;
    }
    if(root->right == nullptr) return; // Nothing to split here

    BinaryNode<Bundle>* t = root->right;
    while (t->left != nullptr){
        t = t->left;
    }
    botSeg = *(t->getElement().getBot());

    if(Predicate::AIsLeftOfB(p.position(),botSeg.support())){
        //p is in the top bundle
        Bundle R = t->getElement().splitTree(Segment_2r_colored(p.getPoint(), p.getOtherPoint(), p.get_color()));
        BinaryNode<Bundle>* new_node = new BinaryNode<Bundle>(R, nullptr, root->right);
        t->right = new_node;
        return;
    }

    //def split(p, struct, color):
    //	switch(locate p in struct):
    //		case (p between two (color) bundles):
    //			continue
    //		case (p inside (color) bundle):
    //			split bundle at p
    //		case (p inside two (color) bundles):
    //			split bundles at p

}


//=============================================================================
// Implementation: BundleList
//=============================================================================
void BundleList::Remove(const std::list<Bundle>::iterator where){
    // Removes the element at location "where"
    blist_.erase(where);
}

void BundleList::Insert(std::list<Bundle>::iterator where, Bundle& to_insert){
    blist_.insert(where, to_insert);
}

void BundleList::LocatePoint(ArrangementVertex_2r &p, Bundle &top, Bundle &bottom){
    // Splits bundles in the bundle list at current vertex, then locates the current vertex among the blue bundles in the bundle tree.
    // top = lowest blue bundle above or ending at p
    // bot = highest blue bundle below or ending at p
    SplitBundles(p);
    std::list<Bundle>::iterator index = FindIndex(p);
    // Check for the bundle below p
    if(index == blist_.begin()) bottom = Bundle();
    else if(index->getTop()->get_color() == p.get_color()){
        bottom = *index;
    }
    else if(std::prev(index) != blist_.begin()) bottom = *(std::prev(index));
    else bottom = Bundle();
    // Now check for the bundle above p
    if(index == blist_.end()){
        top = Bundle();
    }
    else if(std::next(index) == blist_.end()){
        top = Bundle();
    }
    else if(std::next(index)->getTop()->get_color() == p.get_color()){
        top = *index;
    }
    else if(std::next(index) != blist_.end()){
        top = *(std::next(index));
    }

}

void BundleList::SplitBundles(ArrangementVertex_2r &p){
    // Do a linear search through the bundles in the list for a bundle containing p
    for(std::list<Bundle>::iterator ii = blist_.begin(); ii != blist_.end(); ii ++){
        if((*ii).contains(p)){
            Bundle R = (*ii).splitTree(Segment_2r_colored(p.getPoint(), p.getOtherPoint(), p.get_color()));
            blist_.insert(++ii, R);
            return;
        }
    }
}

std::list<Bundle>::iterator BundleList::FindIndex(const Segment_2r_colored& target_segment){
    // Return the index of the largest bundle that is below or containing target_segment
    for(std::list<Bundle>::iterator ii = blist_.begin(); ii != blist_.end(); ii ++){
        //check if target_segment is in current bundle
        if(Predicate::AIsRightOfB(target_segment.p(), ii->getTop()->support()) && Predicate::AIsLeftOfB(target_segment.p(), ii->getBot()->support())){
            return ii;
        }
        else if(Predicate::AIsRightOfB(target_segment.p(), ii->getBot()->support())){
            if(ii == blist_.begin()) return ii;
            else return std::prev(ii);
        }
    }
    return blist_.begin();

}

std::list<Bundle>::iterator BundleList::FindIndex( ArrangementVertex_2r& p){
    Point_2r tmp_point = p.position();
    bool color_of_p = p.get_color();
    Segment_2r_colored tmp_segment = Segment_2r_colored(tmp_point, tmp_point, color_of_p);
    return FindIndex(tmp_segment);
}


//=============================================================================
// Implementation: ArrangementVertex_2r
//=============================================================================

ArrangementVertex_2r::ArrangementVertex_2r(SharedPoint_2r &pt, SharedPoint_2r otherPt, bool color){
    pt_ = pt;
    otherPoint_ = otherPt;
    color_ = color;
}


//=============================================================================
// Implementation: Arrangement_2r
//=============================================================================

Arrangement_2r::~Arrangement_2r() {
    for(auto i = begin(segments_); i != end(segments_); ++i) {
        SigPopVisualSegment_2r(*i);
    }
    for(auto i = begin(vertices_); i != end(vertices_); ++i) {
        SigPopVisualPoint_2r(i->position());
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

const std::list<Segment_2r_colored>& Arrangement_2r::segments() const{
    return segments_;
}

const std::list<ArrangementVertex_2r>& Arrangement_2r::vertices() const{
    return vertices_;
}

} // Namespace DDAD
