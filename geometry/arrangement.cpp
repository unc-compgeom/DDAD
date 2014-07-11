#include "arithmetic.h"
#include "line.h"
#include "intersection.h"
#include "arrangement.h"


namespace DDAD {


int CountIntersections(const Arrangement_2r &A,
                       Visual::IGeometryObserver *observer){
    /* Do this on A
    //# Invariants for the algorithm:
    //#
    //#	- All intersections whose witnesses are left of the sweepline have
    //#    been reported
    //#	- The order of segments along the sweepline is consistent with pushing
    //#    all intersections as far right as possible
    //#
    //# Events:
    //#	- Sweep line reaches an endpoint
    //#
    //# Preconditions:
    //#	- red/red and blue/blue intersections do not exist (such a coloring
    //#    does exist)
    //#
    //# Output:
    //#	- The number of intersections that occur
    //#
    //# Input:
    //#	- set of segments, each colored red or blue
    //#
    */
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
    BundleTree bdt = BundleTree();
    BundleList bdl = BundleList();
    SharedBundle above;
    SharedBundle below;
    bdl.GenerateSentinels(L, bdt);

    //for each event p in L do:
    int endpoint_index = 0; //debug

    for(std::list<ArrangementVertex_2r>::iterator ii = L.begin();
        ii != L.end(); ii ++)
    {
        Segment_2r_colored current_segment =
                Segment_2r_colored(ii->get_point(), ii->get_other_point(),
                                   ii->is_red());
        bdt.LocateVertex(*ii, above, below);
        // We now have the red bundles directly above and below
        //  (or containing) ii

        below = bdl.LocateVertex(*ii, bdt);
        // Locates the highest bundle below or containing the current event

        crossings += bdl.SortPortion(bdl.get_bottom(), bdl.get_top(), *ii);
        // Count the crossings witnessed by the event point

        if(*(ii->get_point()) < *(ii->get_other_point()))
        {
            // Current point is a left-endpoint
            // Search for the point in the linked list
            SharedBundle new_bundle = std::make_shared<Bundle>();
            SharedSegment new_segment =
                    std::make_shared<Segment_2r_colored>(current_segment);
            new_bundle->Insert(new_segment);
            bdt.Find(*ii);
            SharedBundle tmp = bdt.get_root();
            while(*(ii->get_point()) < tmp)
                tmp = tmp->get_prev_bundle();
            if(ii->is_red()) bdt.InsertBundle(new_bundle);
            //            if(tmp != nullptr && *(ii->get_point()) < tmp)
            //                // ii is below all bundles in the tree
            //                bdl.InsertBundle(new_bundle, tmp->get_prev_bundle());
            bdl.InsertBundle(new_bundle, tmp);
        }
        else
        {
            // Point is a right-endpoint, remove its segment from the structure
            // Locate the bundle containing it
            for(SharedBundle jj = bdl.get_bottom();
                (jj != nullptr);
                jj = jj->get_next_bundle())
            {
                if(jj->Contains(*ii))
                {
                    jj->Remove(std::make_shared<Segment_2r_colored>
                               (current_segment));
                    if(jj->CountSegments() == 0)
                    {
                        bdl.RemoveBundle(jj);
                        bdt.RemoveBundle(jj);
                    }
                }
            }
        }

        // Merge any bundles in the list that deserve it
        for(SharedBundle jj = bdl.get_bottom(); jj != bdl.get_top(); )
        {
            if(jj->get_color() == jj->get_next_bundle()->get_color())
            {
                //deal with bundle tree if bundles are red
                if(jj->get_color())
                {
                    bdt.RemoveBundle(jj);
                    bdt.RemoveBundle(jj->get_next_bundle());
                    jj->Merge(jj->get_next_bundle());
                    bdt.InsertBundle(jj);
                }
                else
                    jj->Merge(jj->get_next_bundle());

            }
            jj = jj->get_next_bundle();
        }
        //crossings++;
        endpoint_index++;

    }
    return crossings;
}


//=============================================================================
// Implementation: ArrangementVertex_2r
//=============================================================================

ArrangementVertex_2r::ArrangementVertex_2r(SharedPoint_2r &pt,
                                           SharedPoint_2r &other_point,
                                           bool is_red){
    color_ = is_red;
    point_ = pt;
    other_point_ = other_point;
}


//=============================================================================
// Implementation: Bundle
//=============================================================================

Bundle::Bundle()
{
    top_segment_ = nullptr;
    bottom_segment_ = nullptr;
    next_bundle_ = nullptr;
    prev_bundle_ = nullptr;
    tree_ = SplayTree<Segment_2r_colored>();
}

Bundle::Bundle(SplayTree<Segment_2r_colored>& the_tree)
{
    the_tree.findMax();
    top_segment_ = std::make_shared<Segment_2r_colored>
            (the_tree.getRoot()->getElement());
    the_tree.findMin();
    bottom_segment_ = std::make_shared<Segment_2r_colored>
            (the_tree.getRoot()->getElement());
    next_bundle_ = nullptr;
    prev_bundle_ = nullptr;
    tree_ = the_tree;
}

void Bundle::Insert(SharedSegment new_segment)
{
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

void Bundle::Remove(SharedSegment old_segment)
{
    tree_.remove(*old_segment);
    // Code to delete the bundle if it is empty should exist in
    //both list and tree
}

SharedBundle Bundle::Split(SharedSegment split_here)
{
    SplayTree<Segment_2r_colored> R = tree_.SplitTree(*split_here);

    //    if(R.getRoot() == nullptr) return nullptr;

    SharedBundle new_bundle = std::make_shared<Bundle>(R);

    // Set next_bundle and prev_bundle pointers
    //next_bundle_->get_prev_bundle();
    SharedBundle my_sptr = std::make_shared<Bundle>(*this);
    new_bundle->set_prev_bundle(my_sptr);
    new_bundle->set_next_bundle(next_bundle_);
    if(next_bundle_ != nullptr) next_bundle_->set_prev_bundle(new_bundle);
    next_bundle_ = new_bundle;
    tree_.findMax();
    top_segment_ =
            std::make_shared<Segment_2r_colored>(tree_.getRoot()->getElement());
    tree_.findMin();
    bottom_segment_ =
            std::make_shared<Segment_2r_colored>(tree_.getRoot()->getElement());
    return new_bundle;
}

SharedBundle Bundle::Split(Point_2r& split_here)
{
    SplayTree<Segment_2r_colored> R = tree_.SplitTree(split_here);

    //    if(R.getRoot() == nullptr) return nullptr;

    SharedBundle new_bundle = std::make_shared<Bundle>(R);

    // Set next_bundle and prev_bundle pointers
    //next_bundle_->get_prev_bundle();
    SharedBundle my_sptr = std::make_shared<Bundle>(*this);
    new_bundle->set_prev_bundle(my_sptr);
    new_bundle->set_next_bundle(next_bundle_);
    if(next_bundle_ != nullptr) next_bundle_->set_prev_bundle(new_bundle);
    next_bundle_ = new_bundle;
    tree_.findMax();
    top_segment_ =
            std::make_shared<Segment_2r_colored>(tree_.getRoot()->getElement());
    tree_.findMin();
    bottom_segment_ =
            std::make_shared<Segment_2r_colored>(tree_.getRoot()->getElement());
    return new_bundle;
}

void Bundle::Merge(SharedBundle to_merge)
{
    // Merges to_merge into the current bundle
    SharedBundle my_sptr = to_merge->get_prev_bundle();
    if(to_merge->get_next_bundle() != nullptr){
        to_merge->get_next_bundle()->set_prev_bundle(my_sptr);
    }
    set_next_bundle(to_merge->get_next_bundle());
    top_segment_ = to_merge->get_top_seg();
    tree_.mergeTree(to_merge->get_tree());

}

bool Bundle::Contains(ArrangementVertex_2r &test_point)
{
    Point_2r the_point = *(test_point.get_point());
    return (!Predicate::AIsLeftOfB(the_point, top_segment_->support()) &&
            !Predicate::AIsRightOfB(the_point, bottom_segment_->support()));
}

RelativePosition Bundle::SetRelativePosition(ArrangementVertex_2r &test_point)
{
    Point_2r the_point = *(test_point.get_point());
    if(Predicate::AIsLeftOfB(the_point, top_segment_->support()))
    {
        rel_position_ = BELOW;
    }
    else if(Predicate::AIsRightOfB(the_point, bottom_segment_->support()))
    {
        rel_position_ = ABOVE;
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

void BundleTree::Find(ArrangementVertex_2r& input_vertex)
{
    bundle_tree_.Splay(*input_vertex.get_point(), bundle_tree_.getRoot());
}

void BundleTree::LocateVertex(ArrangementVertex_2r &input_vertex,
                              SharedBundle &above_neighbor,
                              SharedBundle &below_neighbor)
{
    // For an empty tree, our job is easy
    if(bundle_tree_.isEmpty()){
        above_neighbor = nullptr;
        below_neighbor = nullptr;
        return;
    }
    Find(input_vertex);
    // Now either the greatest bundle below input_vertex is at the root, or
    // there is no such bundle and the lowest bundle in the tree is at the root
    if(*input_vertex.get_point() < bundle_tree_.getRoot()->getElement())
    {
        above_neighbor = bundle_tree_.getRoot()->getElement();
        below_neighbor = nullptr;
    }
    else if(*input_vertex.get_point() > bundle_tree_.getRoot()->getElement())
    {
        below_neighbor = bundle_tree_.getRoot()->getElement();
        // Search down the right subtree for the lowest bundle
        //  above input_vertex
        BinaryNode<SharedBundle>* tmp = bundle_tree_.getRoot()->right;
        if(tmp == nullptr) above_neighbor = nullptr;
        else
        {
            while(tmp->left != nullptr) tmp = tmp->left;
            above_neighbor = tmp->getElement();
        }
    }
    else if(bundle_tree_.getRoot()->getElement()->Contains(input_vertex))
    {
        above_neighbor = bundle_tree_.getRoot()->getElement();
        below_neighbor = above_neighbor;
    }
    else
        std::cout << "\nCheck segment comparators! "
                  << "This branch should never be reached\n";
}

void BundleTree::InsertBundle(SharedBundle add_this)
{
    if(add_this->get_color()) bundle_tree_.insert(add_this);
}

void BundleTree::RemoveBundle(SharedBundle remove_this)
{
    bundle_tree_.remove(remove_this);
}

int BundleTree::Size()
{
    return bundle_tree_.Size();
}

SharedBundle BundleTree::SplitBundleAtVertex(ArrangementVertex_2r &split_here)
{

    Find(split_here);  // Rotate the appropriate bundle to the root
    if(bundle_tree_.getRoot()->getElement()->CountSegments() == 1)
        return bundle_tree_.getRoot()->getElement();
    else if(bundle_tree_.getRoot()->getElement()->Contains(split_here)){
        // remove bundle, split it in two, then insert both back into the tree
        SharedBundle tmp_bundle_left = bundle_tree_.getRoot()->getElement();
        SharedBundle tmp_bundle_rt;
        bundle_tree_.remove(tmp_bundle_left);
        tmp_bundle_rt = tmp_bundle_left->Split(*(split_here.get_point()));
        bundle_tree_.insert(tmp_bundle_left);
        bundle_tree_.insert(tmp_bundle_rt);

        return tmp_bundle_rt;

    }
    else{
        return nullptr;
    }
}

void BundleTree::MergeBundles(SharedBundle above, SharedBundle below)
{
    bundle_tree_.remove(above);
    bundle_tree_.remove(below);
    below->Merge(above);
    bundle_tree_.insert(below);
}

//=============================================================================
// Implementation: BundleList
//=============================================================================

void BundleList::GenerateSentinels(std::list<ArrangementVertex_2r> L,
                                   BundleTree& bdt)
{
    // Find the bounding box of the list of vertices
    rational min_x, max_x, min_y, max_y;
    min_x = L.front().get_x();
    max_x = L.back().get_x();
    min_y = L.front().get_y();
    max_y = L.back().get_y();
    for(std::list<ArrangementVertex_2r>::iterator ii = L.begin();
        ii != L.end(); ii++)
    {
        if(ii->get_y() > max_y)
            max_y = ii->get_y();
        else if(ii->get_y() < min_y)
            min_y = ii->get_y();
        else if(ii->get_x() > max_x)
            max_x = ii->get_x();
        else if(ii->get_x() < min_x)
            min_x = ii->get_x();
    }
    // Generate sentinels at the top and bottom of the list
    SharedBundle top_red_sentinel = std::make_shared<Bundle>();
    Segment_2r_colored tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, max_y+(1)),
                std::make_shared<Point_2r>(max_x, max_y+(1)),
                true);
    top_red_sentinel->Insert
                (std::make_shared<Segment_2r_colored>(tmp_segment));

    SharedBundle top_blue_sentinel = std::make_shared<Bundle>();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, max_y+(2)),
                std::make_shared<Point_2r>(max_x, max_y+(2)),
                false);
    top_blue_sentinel->Insert
                (std::make_shared<Segment_2r_colored>(tmp_segment));

    SharedBundle bot_red_sentinel = std::make_shared<Bundle>();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, min_y+(-2)),
                std::make_shared<Point_2r>(max_x, min_y+(-2)),
                true);
    bot_red_sentinel->Insert
                (std::make_shared<Segment_2r_colored>(tmp_segment));

    SharedBundle bot_blue_sentinel = std::make_shared<Bundle>();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, min_y+(-1)),
                std::make_shared<Point_2r>(max_x, min_y+(-1)),
                false);
    bot_blue_sentinel->Insert
                (std::make_shared<Segment_2r_colored>(tmp_segment));

    InsertBundle(bot_red_sentinel, nullptr);
    InsertBundle(bot_blue_sentinel, bot_red_sentinel);
    InsertBundle(top_red_sentinel, bot_blue_sentinel);
    InsertBundle(top_blue_sentinel, top_red_sentinel);
    bdt.InsertBundle(bot_red_sentinel);
    bdt.InsertBundle(top_red_sentinel);
    set_bottom(bot_blue_sentinel);
    set_top(top_red_sentinel);
}

void BundleList::InsertBundle(SharedBundle insert_this, SharedBundle after_this)
{
    if(after_this == nullptr)
    {
        insert_this->set_next_bundle(bottom_);
        bottom_ = insert_this;
    }
    else
    {
        insert_this->set_next_bundle(after_this->get_next_bundle());
        after_this->set_next_bundle(insert_this);
    }

    insert_this->set_prev_bundle(after_this);
    SharedBundle temp = insert_this->get_next_bundle();

    if(temp == nullptr) top_ = insert_this;
    else temp->set_prev_bundle(insert_this);

}

void BundleList::RemoveBundle(SharedBundle remove_this)
{
    if(top_ == remove_this)
        top_ = remove_this->get_prev_bundle();
    if(bottom_ == remove_this)
        bottom_ = remove_this->get_next_bundle();

    SharedBundle prev_bundle = remove_this->get_prev_bundle();
    SharedBundle next_bundle = remove_this->get_next_bundle();

    if(prev_bundle != nullptr)
        prev_bundle->set_next_bundle(next_bundle);
    if(next_bundle != nullptr)
        next_bundle->set_prev_bundle(prev_bundle);
}

SharedBundle BundleList::LocateVertex(ArrangementVertex_2r &input_vertex,
                                      BundleTree& bdt)
{
    // Using the tree and list, locate the highest bundle below or containing
    //  the input vertex
    SharedBundle above, below, out;
    bdt.LocateVertex(input_vertex, above, below);
    out = above->get_next_bundle();
    while(out->SetRelativePosition(input_vertex) == ABOVE)
    {
        out = out->get_prev_bundle();
    }
    if(out->Contains(input_vertex) && out->CountSegments()>1)
        SplitBundleAtVertex(out, input_vertex);
    return out;

}

SharedBundle BundleList::SplitBundleAtVertex(SharedBundle split_bundle,
                                             ArrangementVertex_2r &here)
{
    if(split_bundle->CountSegments()==1) return split_bundle;
    SplayTree<Segment_2r_colored> R =
            split_bundle->get_tree()->SplitTree(*(here.get_point()));

    SharedBundle new_bundle = std::make_shared<Bundle>(R);
//    new_bundle->set_prev_bundle(split_bundle);
    InsertBundle(new_bundle, split_bundle);
    return new_bundle;
}

int BundleList::SortPortion(SharedBundle &begin, SharedBundle &end,
                            ArrangementVertex_2r v)
{
    //    SharedBundle temp_prev = begin->get_prev_bundle();
    //    SharedBundle temp_next = end->get_next_bundle();


    for(SharedBundle kk = bottom_; (kk != top_->get_next_bundle());
        kk = kk->get_next_bundle() )
    {
        kk->SetRelativePosition(v);
    }

    SharedBundle j;
    SharedBundle i;
    int num_intersections = 0;

    for(i = bottom_;
        /*(i != end->get_next_bundle()) && */(i != top_->get_next_bundle());
        i = i->get_next_bundle())
    {
        j = i;

        while(
              (j->get_prev_bundle() != bottom_->get_prev_bundle()) &&
              (j->get_rel_position() < j->get_prev_bundle()->get_rel_position())
              )
        {
            num_intersections += (j->CountSegments())*
                    (j->get_prev_bundle()->CountSegments());
            SwapAdjacentBundles(j->get_prev_bundle(), j);
        }
    }

    return num_intersections;
}

void BundleList::SwapAdjacentBundles(SharedBundle& left, SharedBundle& right)
{
    if(left == bottom_) bottom_ = right;
    else if(right == bottom_) bottom_ = left;
    if(left == top_) top_ = right;
    else if(right == top_) top_ = left;
    SharedBundle tmp = left->get_prev_bundle();
    left->set_prev_bundle(right);
    left->set_next_bundle(right->get_next_bundle());
    if(left->get_next_bundle() != nullptr)
        left->get_next_bundle()->set_prev_bundle(left);
    right->set_prev_bundle(tmp);
    right->set_next_bundle(left);
    if(right->get_prev_bundle() != nullptr)
        right->get_prev_bundle()->set_next_bundle(right);
}

void BundleList::SwapBundles(SharedBundle& a, SharedBundle& b)
{
    SharedBundle tmp = std::make_shared<Bundle>();
    if(a == bottom_) bottom_ = b;
    else if(b == bottom_) bottom_ = a;
    if(a == top_) top_ = b;
    else if(b == top_) top_ = a;
    tmp->set_next_bundle(a->get_next_bundle());
    tmp->set_prev_bundle(a->get_prev_bundle());
    a->set_prev_bundle(b->get_prev_bundle());
    a->set_next_bundle(b->get_next_bundle());
    if(tmp->get_prev_bundle() != nullptr)
        tmp->get_prev_bundle()->set_next_bundle(b);
    if(tmp->get_next_bundle() != nullptr)
        tmp->get_next_bundle()->set_prev_bundle(b);
    b->set_next_bundle(tmp->get_next_bundle());
    b->set_prev_bundle(tmp->get_prev_bundle());
    if(b->get_prev_bundle() != nullptr)
        b->get_prev_bundle()->set_next_bundle(a);
    if(b->get_next_bundle() != nullptr)
        b->get_next_bundle()->set_prev_bundle(a);

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
    vertices_.push_front(ArrangementVertex_2r(std::make_shared<Point_2r>(v),
                                              std::make_shared<Point_2r>(w),
                                              color));
    vertices_.push_front(ArrangementVertex_2r(std::make_shared<Point_2r>(w),
                                              std::make_shared<Point_2r>(v),
                                              color));
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

    SigRegisterPoint_2r(w);
    SigRegisterPoint_2r(v);
    SigPushVisualPoint_2r(v,vPoint);
    SigPushVisualPoint_2r(w,vPoint);
    Segment_2r edge(std::make_shared<Point_2r>(v),
                    std::make_shared<Point_2r>(w));
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
