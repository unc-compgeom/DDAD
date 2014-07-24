#include "arithmetic.h"
#include "line.h"
#include "intersection.h"
#include "arrangement.h"
#include "point.h"


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
    SharedBundle red_above, red_below, blue_above, blue_below, bot, top;
    bdl.GenerateSentinels(L, bdt);

    //for each event p in L do:
    int endpoint_index = 0; //debug

    for(std::list<ArrangementVertex_2r>::iterator ii = L.begin();
        ii != L.end(); ii ++)
    {

        std::cout << "\nCurrent vertex: " << *(ii->get_point());

        bdl.LocateVertex(*ii, bdt, red_above, red_below,
                                 blue_above, blue_below);
        // Locates the highest bundle below or containing the current event
//        top = (blue_above > red_above)? blue_above : red_above;
//        bot = (blue_below < red_below)? blue_below : red_below;
        bot = bdl.get_bottom()->get_prev_bundle();
        top = bdl.get_top()->get_next_bundle();
        // This isn't quite right, but it might be close enough

        bdl.PrintState(bdl.get_bottom(), bdl.get_top());

        bdl.SplitBundlesContaining(*ii, bdt, top, bot);
        // Split any bundles containing the current vertex (must be in
        //  range [bottom, top]


        crossings += bdl.SortPortion(*ii, bot, top);
        // Count the crossings witnessed by the event point

        if(*(ii->get_point()) < *(ii->get_other_point()))
            // Current point is a left-endpoint
            // Search for the point in the linked list
            bdl.InsertLeftEndpoint(*ii, bdt);
        else
            // Point is a right-endpoint, remove its segment from the structure
            // Locate the bundle containing it
            bdl.RemoveRightEndpoint(*ii, bdt);

        // Merge any bundles in the list that deserve it
        bdl.MergeOrderedBundles(bdt);

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
                                           bool is_red,
                                           SharedSegment& my_segment){
    color_ = is_red;
    point_ = pt;
    other_point_ = other_point;
    my_segment_ = my_segment;
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
    tree_ = SplayTree<SharedSegment>();
    size_ = 0;
}

Bundle::Bundle(SplayTree<SharedSegment>& the_tree)
{
    the_tree.findMax();
    top_segment_ = the_tree.getRoot()->getElement();
    the_tree.findMin();
    bottom_segment_ = the_tree.getRoot()->getElement();
    next_bundle_ = nullptr;
    prev_bundle_ = nullptr;
    tree_ = the_tree;
    size_ = the_tree.Size();
}

void Bundle::Insert(SharedSegment new_segment)
{
    // Make sure segments are inserted in right-left order
    if(new_segment->q() < new_segment->p()){
        Point_2r new_p = new_segment->q();
        Point_2r new_q = new_segment->p();
        bool new_color = new_segment->get_color();
        tree_.insert(std::make_shared<Segment_2r_colored>(new_p, new_q, new_color));
    }
    else{
        tree_.insert(new_segment);
    }
    // Make sure top_segment and bottom_segment have been set
    if(top_segment_ == nullptr && bottom_segment_ == nullptr){
        top_segment_ = bottom_segment_ = new_segment;
    }
    else{
        if(Predicate::SegmentAIsBelowB(*top_segment_, *new_segment)) top_segment_ = new_segment;
        if(Predicate::SegmentAIsBelowB(*new_segment, *bottom_segment_)) bottom_segment_ = new_segment;
    }
    size_ = tree_.Size();
}

void Bundle::Remove(SharedSegment old_segment)
{
    tree_.remove(old_segment);
    size_ = tree_.Size();
    // Code to delete the bundle if it is empty should exist in
    //both list and tree
}

SharedBundle Bundle::Split(SharedSegment split_here,
                           SharedBundle& my_sptr)
{
    Point_2r copy_point = split_here->p();
    return Split(copy_point, my_sptr);
}

SharedBundle Bundle::Split(Point_2r& split_here, SharedBundle& my_sptr)
{
    if(CountSegments() == 1) return my_sptr;
    SplayTree<SharedSegment> R = tree_.SplitTree(split_here);
    SharedBundle new_bundle = std::make_shared<Bundle>(R);
    // Set next_bundle and prev_bundle pointers
    new_bundle->set_prev_bundle(my_sptr);
    new_bundle->set_next_bundle(next_bundle_);
    if(next_bundle_ != nullptr) next_bundle_->set_prev_bundle(new_bundle);
    next_bundle_ = new_bundle;
    tree_.findMax();
    top_segment_ = tree_.getRoot()->getElement();
    tree_.findMin();
    bottom_segment_ = tree_.getRoot()->getElement();
    size_ = tree_.Size();
    new_bundle->set_size(R.Size());
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
    size_ = tree_.Size();

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
        rel_position_ = BELOW;
    else if(Predicate::AIsRightOfB(the_point, bottom_segment_->support()))
        rel_position_ = ABOVE;
    else
        rel_position_ = ENDING;
    return rel_position_;
}

int Bundle::CountSegments()
{
    return tree_.Size();
}


//=============================================================================
// Implementation: BundleTree
//=============================================================================

SharedBundle BundleTree::Find(ArrangementVertex_2r& input_vertex)
{
    bundle_tree_.Splay(*input_vertex.get_point(), bundle_tree_.getRoot());
    return bundle_tree_.getRoot()->getElement();
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
        tmp_bundle_rt = tmp_bundle_left->Split(*(split_here.get_point()), tmp_bundle_left);
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
        if(ii->get_x() > max_x)
            max_x = ii->get_x();
        else if(ii->get_x() < min_x)
            min_x = ii->get_x();
    }
    /* Generate sentinels at the top and bottom of the list
        (order is blue-red-blue-red-blue from bottom to top) */
    SharedBundle top_blue_cap = std::make_shared<Bundle>();
    Segment_2r_colored tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, max_y+3),
                std::make_shared<Point_2r>(max_x, max_y+3),
                false);
    top_blue_cap->Insert
            (std::make_shared<Segment_2r_colored>(tmp_segment));

    SharedBundle top_red_cap = std::make_shared<Bundle>();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, max_y+4),
                std::make_shared<Point_2r>(max_x, max_y+4),
                true);
    top_red_cap->Insert(std::make_shared<Segment_2r_colored>(tmp_segment));

    SharedBundle top_red_sentinel = std::make_shared<Bundle>();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, max_y+(2)),
                std::make_shared<Point_2r>(max_x, max_y+(2)),
                true);
    top_red_sentinel->Insert
            (std::make_shared<Segment_2r_colored>(tmp_segment));
    //    std::cout << "\nTop red: " << tmp_segment.p() << tmp_segment.q();

    SharedBundle top_blue_sentinel = std::make_shared<Bundle>();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, max_y+(1)),
                std::make_shared<Point_2r>(max_x, max_y+(1)),
                false);
    top_blue_sentinel->Insert
            (std::make_shared<Segment_2r_colored>(tmp_segment));
    //    std::cout << "\nTop blue: " << tmp_segment.p() << tmp_segment.q();

    SharedBundle bot_red_sentinel = std::make_shared<Bundle>();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, min_y+(-1)),
                std::make_shared<Point_2r>(max_x, min_y+(-1)),
                true);
    bot_red_sentinel->Insert
            (std::make_shared<Segment_2r_colored>(tmp_segment));
    //    std::cout << "\nBot red: " << tmp_segment.p() << tmp_segment.q();

    SharedBundle bot_blue_sentinel = std::make_shared<Bundle>();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, min_y+(-2)),
                std::make_shared<Point_2r>(max_x, min_y+(-2)),
                false);
    bot_blue_sentinel->Insert
            (std::make_shared<Segment_2r_colored>(tmp_segment));
    //    std::cout << "\nBot blue: " << tmp_segment.p() << tmp_segment.q();

    SharedBundle bot_red_cap = std::make_shared<Bundle>();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, min_y-3),
                std::make_shared<Point_2r>(max_x, min_y-3),
                true);
    bot_red_cap->Insert(std::make_shared<Segment_2r_colored>(tmp_segment));

    SharedBundle bot_blue_cap = std::make_shared<Bundle>();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, min_y-4),
                std::make_shared<Point_2r>(max_x, min_y-4),
                false);
    bot_blue_cap->Insert(std::make_shared<Segment_2r_colored>(tmp_segment));

    InsertBundle(bot_blue_cap, nullptr);
    InsertBundle(bot_red_cap, bot_blue_cap);
    InsertBundle(bot_blue_sentinel, bot_red_cap);
    InsertBundle(bot_red_sentinel, bot_blue_sentinel);
    InsertBundle(top_blue_sentinel, bot_red_sentinel);
    InsertBundle(top_red_sentinel, top_blue_sentinel);
    InsertBundle(top_blue_cap, top_red_sentinel);
    InsertBundle(top_red_cap, top_blue_cap);
    bdt.InsertBundle(bot_red_cap);
    bdt.InsertBundle(bot_red_sentinel);
    bdt.InsertBundle(top_red_sentinel);
    bdt.InsertBundle(top_red_cap);
    set_bottom(bot_red_sentinel);
    set_top(top_blue_sentinel);
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

    //    std::cout << get_bottom()->get_bot_seg()->p();
    //    std::cout << get_bottom()->get_bot_seg()->q();

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

void BundleList::LocateVertex(ArrangementVertex_2r &input_vertex,
                              BundleTree& bdt,
                              SharedBundle& red_above,
                              SharedBundle& red_below,
                              SharedBundle& blue_above,
                              SharedBundle& blue_below)
{
    // Using the tree and list, locate the highest bundle below or containing
    //  the input vertex
    // Locate red bundles within the tree
    red_below = bdt.Find(input_vertex);
    red_above = red_below->get_next_bundle()->get_next_bundle();
    if(red_below->Contains(input_vertex))
        red_below = red_below->get_prev_bundle()->get_prev_bundle();

    // Locate blue bundles using the list
    blue_below = red_below->get_next_bundle();
    blue_above = red_below->get_prev_bundle();
    while(blue_below->SetRelativePosition(input_vertex) != BELOW)
//    while(*(input_vertex.get_point()) <= blue_below)
        blue_below = blue_below->get_prev_bundle()->get_prev_bundle();
    while(blue_above->SetRelativePosition(input_vertex) != ABOVE)
//    while(*(input_vertex.get_point()) >= blue_above)
        blue_above = blue_above->get_next_bundle()->get_next_bundle();
}

SharedBundle BundleList::SplitBundleAtVertex(SharedBundle split_bundle,
                                             ArrangementVertex_2r &here)
{
    if(split_bundle->CountSegments()==1) return split_bundle;
    SharedBundle new_bundle = split_bundle->Split(*(here.get_point()),
                                                  split_bundle);
//    SplayTree<SharedSegment> R =
//            split_bundle->get_tree()->SplitTree(*(here.get_point()));

//    SharedBundle new_bundle = std::make_shared<Bundle>(R);
//    //    new_bundle->set_prev_bundle(split_bundle);
    if(split_bundle == top_) top_ = new_bundle;
//    InsertBundle(new_bundle, split_bundle);
    return new_bundle;
}

void BundleList::SplitBundlesContaining(ArrangementVertex_2r& input_vertex,
                                        BundleTree& bdt,
                                        SharedBundle& top,
                                        SharedBundle& bot)
{
    for(SharedBundle ii = bot; ii != top; ii = ii->get_next_bundle())
    {
        if(ii->Contains(input_vertex))
        {
            if(ii->get_color())
                bdt.SplitBundleAtVertex(input_vertex);
            else
                SplitBundleAtVertex(ii, input_vertex);
            if(top_ == ii)
                top_ = ii->get_next_bundle();
        }
    }
}

int BundleList::SortPortion(ArrangementVertex_2r& v, SharedBundle& begin,
                            SharedBundle& end)
{
    for(SharedBundle kk = begin; (kk != end->get_next_bundle());
        kk = kk->get_next_bundle() )
    {
        kk->SetRelativePosition(v);
    }
    SharedBundle j;
    SharedBundle i;
    int num_intersections = 0;

    for(i = begin; (i != end->get_next_bundle()); i = i->get_next_bundle())
    {
        j = i;
        while(
              (j->get_prev_bundle() != begin->get_prev_bundle()) &&
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

void BundleList::InsertLeftEndpoint(ArrangementVertex_2r& input_vertex,
                                    BundleTree& bdt)
{

    SharedBundle new_bundle = std::make_shared<Bundle>();
    Segment_2r_colored current_segment =
            Segment_2r_colored(input_vertex.get_point(),
                               input_vertex.get_other_point(),
                               input_vertex.is_red());
    SharedSegment new_segment =
            std::make_shared<Segment_2r_colored>(current_segment);
    new_bundle->Insert(new_segment);
    bdt.Find(input_vertex);
    SharedBundle tmp = bdt.get_root();
    while(DDAD::AIsBelowB(*(input_vertex.get_point()), tmp))
        tmp = tmp->get_prev_bundle();
    if(input_vertex.is_red()) bdt.InsertBundle(new_bundle);
    //            if(tmp != nullptr && *(ii->get_point()) < tmp)
    //                // ii is below all bundles in the tree
    //                bdl.InsertBundle(new_bundle, tmp->get_prev_bundle());
    InsertBundle(new_bundle, tmp);
}

void BundleList::RemoveRightEndpoint(ArrangementVertex_2r& input_vertex,
                                     BundleTree& bdt)
{
    for(SharedBundle jj = get_bottom();
        (jj != nullptr);
        jj = jj->get_next_bundle())
    {
        if(jj->Contains(input_vertex))
        {
            SharedSegment current_segment = input_vertex.get_segment();
            jj->Remove(current_segment);
            if(jj->CountSegments() == 0)
            {
                RemoveBundle(jj);
                bdt.RemoveBundle(jj);
            }
        }
    }
}

void BundleList::MergeOrderedBundles(BundleTree& bdt)
{
    for(SharedBundle jj = get_bottom(); jj != get_top()->get_next_bundle(); )
    {
        SharedBundle tmp;
        while(jj->get_color() == jj->get_next_bundle()->get_color())
        {
            tmp = jj->get_next_bundle();
            //deal with bundle tree if bundles are red
            if(jj->get_color())
            {
                bdt.RemoveBundle(jj);
                bdt.RemoveBundle(tmp);
                jj->Merge(tmp);
                bdt.InsertBundle(jj);
            }
            else
                jj->Merge(jj->get_next_bundle());
            // reset top if need be
            if(tmp == top_)
                top_ = jj;
        }
        jj = jj->get_next_bundle();
    }
}

void BundleList::SwapBundles(SharedBundle& a, SharedBundle& b)
{
    SharedBundle tmp = std::make_shared<Bundle>();
    if(a == bottom_)
        bottom_ = b;
    else if(b == bottom_)
        bottom_ = a;
    if(a == top_)
        top_ = b;
    else if(b == top_)
        top_ = a;
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

void BundleList::PrintState(SharedBundle &start, SharedBundle &end)
{
    std::string current_color, bp, bq, tp, tq;
    int nsegments;
    for(SharedBundle tmp = start; tmp != end->get_next_bundle(); tmp = tmp->get_next_bundle())
    {
        current_color = (tmp->get_color()) ? "RED" : "BLUE";
        tp = to_string(tmp->get_top_seg()->p());
        tq = to_string(tmp->get_top_seg()->q());
        bp = to_string(tmp->get_bot_seg()->p());
        bq = to_string(tmp->get_bot_seg()->q());
        nsegments =tmp->CountSegments();
        printf("\n%8.8s -> %8.8s | %8.8s -> %8.8s | %4.4s | %2.2i",
               bp.c_str(), bq.c_str(), tp.c_str(), tq.c_str(),
               current_color.c_str(), nsegments);
    }
    printf("\n");
}


//=============================================================================
// Implementation: Arrangement_2r
//=============================================================================

Arrangement_2r::~Arrangement_2r() {
    for(auto i = begin(segments_); i != end(segments_); ++i) {
        SigPopVisualSegment_2r(**i);
    }
    for(auto i = begin(vertices_); i != end(vertices_); ++i) {
        SigPopVisualPoint_2r(*(i->get_point()));
    }
}

void Arrangement_2r::AddSegment(Point_2r& v, Point_2r& w, bool color){
    //    LOG(INFO) << "in AddSegment";
    SharedSegment new_segment = (v < w) ?
                std::make_shared<Segment_2r_colored>(v, w, color) :
                std::make_shared<Segment_2r_colored>(w, v, color);
    segments_.push_front(new_segment);
    vertices_.push_front(ArrangementVertex_2r(std::make_shared<Point_2r>(v),
                                              std::make_shared<Point_2r>(w),
                                              color,
                                              new_segment));
    vertices_.push_front(ArrangementVertex_2r(std::make_shared<Point_2r>(w),
                                              std::make_shared<Point_2r>(v),
                                              color,
                                              new_segment));
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


} // Namespace DDAD
