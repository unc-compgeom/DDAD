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

    // Draw a box around the arrangement
    rational min_x = L.front().get_x();
    rational min_y = L.front().get_y();
    rational max_x = L.back().get_x();
    rational max_y = L.back().get_y();
    for(std::list<ArrangementVertex_2r>::iterator ii = L.begin(); ii != L.end(); ii++){
        if(ii->get_y() > max_y) max_y = ii->get_y();
        if(ii->get_y() < min_y) min_y = ii->get_y();
    }
    Point_2r top_left = Point_2r(min_x - rational(1), max_y + rational(1));
    Point_2r top_right = Point_2r(max_x + rational(1), max_y + rational(1));
    Point_2r bot_left = Point_2r(min_x - rational(1), min_y - rational(1));
    Point_2r bot_right = Point_2r(max_x + rational(1), min_y - rational(1));

    // Initialize data structures
    BundleTree bdt = BundleTree();
    BundleList bdl = BundleList();

    SharedBundle top_blue = std::make_shared<Bundle>();
    top_blue->Insert(std::make_shared<Segment_2r_colored>(top_left,
                                                          top_right, false));
    top_blue->Insert(std::make_shared<Segment_2r_colored>(bot_left,
                                                          bot_right, false));

    top_left = Point_2r(min_x - rational(2), max_y + rational(2));
    top_right = Point_2r(max_x + rational(2), max_y + rational(2));
    bot_left = Point_2r(min_x - rational(2), min_y - rational(2));
    bot_right = Point_2r(max_x + rational(2), min_y - rational(2));

    // Create a red bundle above the arrangement, a red bundle below the
    //arrangement, and a blue bundle sandwiched between them
    SharedBundle top_red = std::make_shared<Bundle>();
    top_red->Insert(std::make_shared<Segment_2r_colored>(top_left,
                                                         top_right, true));
    SharedBundle bot_red = std::make_shared<Bundle>();
    bot_red->Insert(std::make_shared<Segment_2r_colored>(bot_left,
                                                         bot_right, true));
    SharedBundle bot_blue = top_blue;
    bdt.InsertBundle(top_red);
    bdt.InsertBundle(bot_red);
    top_red->set_next_bundle(bot_red);
    top_red->set_prev_bundle(top_blue);
    bot_red->set_next_bundle(top_red);
    bot_red->set_prev_bundle(top_red);
    bdl.InsertBundle(bot_blue,bot_red);
    bdl.set_back(top_red);
    bdl.set_front(bot_red);
    //for each event p in L do:

    for(std::list<ArrangementVertex_2r>::iterator ii = L.begin(); ii != L.end(); ii ++){

        Segment_2r_colored current_segment =
                Segment_2r_colored(ii->get_point(), ii->get_other_point(),
                                   ii->is_red());
        bdt.LocateVertex(*ii, top_red, bot_red);
        // We now have the red bundles directly above and below (or containing) ii


        // Split any bundles that might contain p, and update the tree/list
        if(top_red == bot_red)
        {
            //get blue bundles that sandwich the red bundle
            top_blue = top_red->get_next_bundle();
            bot_blue = bot_red->get_prev_bundle();
            bdt.SplitBundleAtVertex(*ii);
        }
        else if(bot_blue->Contains(*ii))
        {
            bot_blue = top_red->get_prev_bundle();
            top_blue = bdl.SplitBundleAtVertex(bot_blue, *ii);
            top_blue->SetRelativePosition(*ii);
            bot_blue->SetRelativePosition(*ii);
        }
        //After this point, bundles should be properly labeled as above, below, or ending at ii

       crossings += bdl.SortPortion(bot_blue, top_blue);



        // Check on the order of the bundles around the current point

        //	if bundles r and b in wrong positions:
        //		countIntersections(rxb)
        //		swap(r,b)
        // repair brundletree by merge
        // repair brundlelist
        // Insert any new bundles
        if(*(ii->get_point()) < *(ii->get_other_point())){ // Current point is a left-endpoint
            // Search for the point in the linked list
            SharedBundle new_bundle = std::make_shared<Bundle>();
            new_bundle->Insert(std::make_shared<Segment_2r_colored>(current_segment));

            if(ii->is_red()) bdt.InsertBundle(new_bundle);
            else bdt.Find(*ii);
            bdl.InsertBundle(new_bundle, bdt.get_root());
//            std::list<Bundle>::iterator pos_in_list = bdl.FindIndex(current_segment);
//            bdl.Insert(pos_in_list, new_bundle);
//            bdt.insert(new_bundle);
        }

        // Merge any bundles in the list that deserve it
        for(SharedBundle jj = bot_blue->get_prev_bundle();
            jj != top_blue->get_next_bundle();
            jj = jj->get_next_bundle())
        {
            if(jj->get_color() == jj->get_next_bundle()->get_color()){
                jj->Merge(jj->get_next_bundle());
            }
        }

        crossings++;
    }


    return crossings;
}


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
    SharedBundle my_sptr = next_bundle_->get_prev_bundle();
    new_bundle->set_prev_bundle(my_sptr);
    new_bundle->set_next_bundle(next_bundle_);
    if(next_bundle_ != nullptr) next_bundle_->set_prev_bundle(new_bundle);
    next_bundle_ = new_bundle;
    tree_.findMax();
    top_segment_ = std::make_shared<Segment_2r_colored>(tree_.getRoot()->getElement());
    tree_.findMin();
    bottom_segment_ = std::make_shared<Segment_2r_colored>(tree_.getRoot()->getElement());
    return new_bundle;
}

void Bundle::Merge(SharedBundle to_merge){
    // Merges to_merge into the current bundle
    SharedBundle my_sptr = next_bundle_->get_prev_bundle();
    if(to_merge->get_next_bundle() != nullptr){
        to_merge->get_next_bundle()->set_prev_bundle(my_sptr);
    }
    if(to_merge->get_prev_bundle() != nullptr){
        to_merge->get_prev_bundle()->set_next_bundle(my_sptr);
    }
    tree_.mergeTree(to_merge->get_tree());

}

bool Bundle::Contains(ArrangementVertex_2r &test_point){
    Point_2r the_point = *(test_point.get_point());
    return (Predicate::AIsRightOfB(the_point, top_segment_->support()) &&
            Predicate::AIsLeftOfB(the_point, bottom_segment_->support()));
}

RelativePosition Bundle::SetRelativePosition(ArrangementVertex_2r &test_point){
    Point_2r the_point = *(test_point.get_point());
    if(Predicate::AIsLeftOfB(the_point, top_segment_->support()))
    {
        rel_position_ = ABOVE;
    }
    else if(Predicate::AIsRightOfB(the_point, bottom_segment_->support()))
    {
        rel_position_ = BELOW;
    }
    else
    {
        rel_position_ = ENDING;
    }
    return rel_position_;
}

int Bundle::CountSegments()
{
    return tree_.Size();
}


//=============================================================================
// Implementation: BundleTree
//=============================================================================

void BundleTree::Find(ArrangementVertex_2r& input_vertex){
    Segment_2r_colored tmp_segment =
            Segment_2r_colored(input_vertex.get_point(),
                               input_vertex.get_other_point(),
                               input_vertex.is_red());
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

void BundleTree::InsertBundle(SharedBundle add_this)
{
    bundle_tree_.insert(add_this);
}

void BundleTree::RemoveBundle(SharedBundle remove_this)
{
    bundle_tree_.remove(remove_this);
}

SharedBundle BundleTree::SplitBundleAtVertex(ArrangementVertex_2r &split_here)
{
    Find(split_here);  // Rotate the appropriate bundle to the root
    if(bundle_tree_.getRoot()->getElement()->Contains(split_here)){
        // Create a new bundle from the split
        SharedSegment tmp_segment = std::make_shared<Segment_2r_colored>(split_here.get_point(), split_here.get_other_point(), split_here.is_red());
        SharedBundle new_bundle = bundle_tree_.getRoot()->element->Split(tmp_segment);
        bundle_tree_.getRoot()->element->SetRelativePosition(split_here);
        new_bundle->SetRelativePosition(split_here);
        return new_bundle;
    }
    else{
        return nullptr;
    }
}

//=============================================================================
// Implementation: BundleList
//=============================================================================

void BundleList::InsertBundle(SharedBundle insert_this, SharedBundle after_this)
{
    insert_this->set_next_bundle(after_this->get_next_bundle());
    insert_this->set_prev_bundle(after_this);
    after_this->set_next_bundle(insert_this);
    insert_this->get_next_bundle()->set_prev_bundle(insert_this);
}

void BundleList::RemoveBundle(SharedBundle remove_this)
{
    SharedBundle prev_bundle = remove_this->get_prev_bundle();
    SharedBundle next_bundle = remove_this->get_next_bundle();

    prev_bundle->set_next_bundle(next_bundle);
    next_bundle->set_prev_bundle(prev_bundle);
}

SharedBundle BundleList::SplitBundleAtVertex(SharedBundle split_bundle,
                                     ArrangementVertex_2r &here)
{
    SharedSegment tmp_segment = std::make_shared<Segment_2r_colored>(
                here.get_point(), here.get_other_point(), here.is_red());
    SharedBundle new_bundle = split_bundle->Split(tmp_segment);
    new_bundle->SetRelativePosition(here);
    split_bundle->SetRelativePosition(here);
    InsertBundle(new_bundle, split_bundle);
    return new_bundle;
}

int BundleList::SortPortion(SharedBundle begin, SharedBundle end)
{
    SharedBundle j;
    int num_intersections = 0;

    for(SharedBundle i = begin; i != end; i = i->get_next_bundle())
    {
        j = i;

        while(j != begin && (j->get_rel_position() < j->get_prev_bundle()->get_rel_position())){
            num_intersections += (j->CountSegments())*(j->get_prev_bundle()->CountSegments());
            SwapBundles(j, j->get_prev_bundle());
            j = j->get_prev_bundle();
        }
    }

    return num_intersections;
}

void BundleList::SwapAdjacentBundles(SharedBundle a, SharedBundle b)
{
    SharedBundle first = a->get_prev_bundle();
    SharedBundle last = b->get_next_bundle();
    first->set_next_bundle(b);
    last->set_prev_bundle(a);
    a->set_next_bundle(last);
    b->set_prev_bundle(first);
    a->set_prev_bundle(b);
    b->set_next_bundle(a);

}

void BundleList::SwapBundles(SharedBundle a, SharedBundle b){
    SharedBundle tmp;
    a->get_prev_bundle()->set_next_bundle(b);
    a->get_next_bundle()->set_prev_bundle(b);
    b->get_prev_bundle()->set_next_bundle(a);
    b->get_next_bundle()->set_prev_bundle(a);
    tmp = a->get_prev_bundle();
    a->set_prev_bundle(b->get_prev_bundle());
    b->set_prev_bundle(tmp);
    tmp = a->get_next_bundle();
    a->set_next_bundle(b->get_next_bundle());
    b->set_next_bundle(tmp);
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
