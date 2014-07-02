#include "arithmetic.h"
#include "line.h"
#include "intersection.h"
#include "arrangement.h"


namespace DDAD {


int CountIntersections(const Arrangement_2r &A,
                       Visual::IGeometryObserver *observer){
    // Do this on A
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
    DDAD::BundleTree bdt = BundleTree();
    DDAD::BundleList bdl = BundleList();
    DDAD::SharedBundle above;
    DDAD::SharedBundle below;

    //for each event p in L do:


    for(std::list<ArrangementVertex_2r>::iterator ii = L.begin();
        ii != L.end(); ii ++)
    {
        Segment_2r_colored current_segment =
                Segment_2r_colored(ii->get_point(), ii->get_other_point(),
                                   ii->is_red());
        bdt.LocateVertex(*ii, above, below);
        // We now have the red bundles directly above and below
        //  (or containing) ii

        //no red bundles, 1 or 0 blue bundles
        if((above == nullptr) && (below == nullptr))
        {
            //
            if(bdl.get_bottom() != nullptr && bdl.get_bottom()->Contains(*ii))
            {
                above = bdl.SplitBundleAtVertex(bdl.get_bottom(), *ii);
                below = above->get_prev_bundle();

            }
            else if((bdl.get_bottom() != nullptr) &&
                    (bdl.get_bottom()->SetRelativePosition(*ii)==BELOW))
            {
                below = bdl.get_bottom();
                above = nullptr;
            }
            else if((bdl.get_bottom() != nullptr) &&
                    (bdl.get_bottom()->SetRelativePosition(*ii)==ABOVE))
            {
                below = nullptr;
                above = bdl.get_bottom();
            }

        }
        //in a red bundle
        else if(above == below)
        {
            above = bdt.SplitBundleAtVertex(*ii);
            if(above->get_next_bundle() != nullptr)
                above = above->get_next_bundle();
            if(below->get_prev_bundle() != nullptr)
                below = below->get_prev_bundle();

        }
        //above all red bundles
        else if(above == nullptr)
        {
            //in top blue bundle
            if(below->Contains(*ii))
                above = bdl.SplitBundleAtVertex(below, *ii);
            if(below->get_prev_bundle() != nullptr)
                below = below->get_prev_bundle();

        }
        //below all red bundles
        else if(below == nullptr)
        {
            //in bottom blue bundle
            if(above->Contains(*ii))
            {
                below = above;
                above = bdl.SplitBundleAtVertex(above, *ii);
            }

            if(above->get_next_bundle() != nullptr)
                above = above->get_next_bundle();
        }
        //between two red bundles
        else
        {
            //in a blue bundle
            if(below->get_next_bundle()->Contains(*ii))
            {
                above = bdl.SplitBundleAtVertex(below->get_next_bundle(), *ii);
            }
            if(above->get_next_bundle() != nullptr)
                above = above->get_next_bundle();
            if(below->get_prev_bundle() != nullptr)
                below = below->get_prev_bundle();
        }



        //After this point, bundles should be properly labeled as above,
        // below, or ending at ii
        SharedBundle start;
        if(below == nullptr) start = bdl.get_bottom();
        else if(below->get_prev_bundle() == nullptr) start = below;
        else start = below->get_prev_bundle();

        SharedBundle end;
        if(above == nullptr) end = bdl.get_top();
        else if(above->get_next_bundle() == nullptr) end = above;
        else end = above->get_next_bundle();

        if(start != nullptr)
            crossings += bdl.SortPortion(start, end, *ii);



        // Check on the order of the bundles around the current point

        //	if bundles r and b in wrong positions:
        //		countIntersections(rxb)
        //		swap(r,b)
        // repair brundletree by merge
        // repair brundlelist
        // Insert any new bundles
        if(*(ii->get_point()) < *(ii->get_other_point()))
        {
            // Current point is a left-endpoint
            // Search for the point in the linked list
            SharedBundle new_bundle = std::make_shared<Bundle>();
            SharedSegment new_segment =
                    std::make_shared<Segment_2r_colored>(current_segment);
            new_bundle->Insert(new_segment);

            if(ii->is_red())
            {
                bdt.InsertBundle(new_bundle);
                if(new_bundle->get_next_bundle() == nullptr)
                    bdl.set_top(new_bundle);
                if(new_bundle->get_prev_bundle() == nullptr)
                    bdl.set_bottom(new_bundle);
            }
            else
            {
                if(bdt.is_empty())
                    bdl.InsertBundle(new_bundle, nullptr);
                else
                {
                    bdt.Find(*ii);
                    bdl.InsertBundle(new_bundle, bdt.get_root());

                }
            }
        }

        // Merge any bundles in the list that deserve it


        for(SharedBundle jj = start;
            (jj != end)&&(jj != nullptr);
            jj = jj->get_next_bundle())
        {
            if(jj->get_next_bundle() != nullptr)
            {
                if(jj->get_color() == jj->get_next_bundle()->get_color()){
                    jj->Merge(jj->get_next_bundle());
                }
            }
        }

        //crossings++;
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
    SplayTree<Segment_2r_colored> R = tree_.splitTree(*split_here);
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
    SharedBundle my_sptr = nullptr;
    if(next_bundle_ != nullptr) my_sptr = next_bundle_->get_prev_bundle();

    if(to_merge->get_next_bundle() != nullptr){
        to_merge->get_next_bundle()->set_prev_bundle(my_sptr);
    }
    if(to_merge->get_prev_bundle() != nullptr){
        to_merge->get_prev_bundle()->set_next_bundle(my_sptr);
    }
    tree_.mergeTree(to_merge->get_tree());

}

bool Bundle::Contains(ArrangementVertex_2r &test_point)
{
    Point_2r the_point = *(test_point.get_point());
    return (Predicate::AIsRightOfB(the_point, top_segment_->support()) &&
            Predicate::AIsLeftOfB(the_point, bottom_segment_->support()));
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
    Segment_2r_colored tmp_segment =
            Segment_2r_colored(input_vertex.get_point(),
                               input_vertex.get_other_point(),
                               input_vertex.is_red());
    SharedBundle tmp_bundle = std::make_shared<Bundle>();
    tmp_bundle->Insert(std::make_shared<Segment_2r_colored>(tmp_segment));
    bundle_tree_.find(tmp_bundle);
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
    Segment_2r_colored tmp_segment =
            Segment_2r_colored(input_vertex.get_point(),
                               input_vertex.get_other_point(),
                               input_vertex.is_red());
    SharedBundle tmp_bundle = std::make_shared<Bundle>();
    tmp_bundle->Insert(std::make_shared<Segment_2r_colored>(tmp_segment));

    // Now either the uppermost bundle below or containing the input vertex
    //  is now at the root, or no such bundles exist and the lowest
    //  bundle in the tree is now at the root
    if(*(bundle_tree_.getRoot()->getElement()) > *(tmp_bundle)){
        // input_vertex is below the entire tree
        above_neighbor = bundle_tree_.getRoot()->getElement();
        below_neighbor = nullptr;
    }
    else{
        // search down the tree for the lowest bundle above input_vertex
        below_neighbor = bundle_tree_.getRoot()->getElement();
        BinaryNode<SharedBundle>* tmp_ptr = bundle_tree_.getRoot();
        //if the vertex is inside a bundle, return that bundle
        if(below_neighbor->Contains(input_vertex))
            above_neighbor = below_neighbor;
        else if(tmp_ptr->right == nullptr)
            above_neighbor = nullptr;
        else
        {
            while(tmp_ptr->left != nullptr) tmp_ptr = tmp_ptr->left;
            above_neighbor = tmp_ptr->getElement();
        }
    }
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
    if(bundle_tree_.getRoot()->getElement()->Contains(split_here)){
        // remove bundle, split it in two, then insert both back into the tree
        SharedSegment tmp_segment =
                std::make_shared<Segment_2r_colored>
                (split_here.get_point(), split_here.get_other_point(),
                 split_here.is_red());
        SharedBundle tmp_bundle_left = bundle_tree_.getRoot()->getElement();
        SharedBundle tmp_bundle_rt;
        bundle_tree_.remove(tmp_bundle_left);
        tmp_bundle_rt = tmp_bundle_left->Split(tmp_segment);
        bundle_tree_.insert(tmp_bundle_left);
        bundle_tree_.insert(tmp_bundle_rt);

        return tmp_bundle_rt;

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

SharedBundle BundleList::SplitBundleAtVertex(SharedBundle split_bundle,
                                             ArrangementVertex_2r &here)
{
    SharedSegment tmp_segment = std::make_shared<Segment_2r_colored>(
                here.get_point(), here.get_other_point(), here.is_red());
    SharedBundle new_bundle = split_bundle->Split(tmp_segment);
    InsertBundle(new_bundle, split_bundle);
    return new_bundle;
}

int BundleList::SortPortion(SharedBundle &begin, SharedBundle &end,
                            ArrangementVertex_2r v)
{
    SharedBundle temp_prev = begin->get_prev_bundle();
    SharedBundle temp_next = end->get_next_bundle();
    begin->set_prev_bundle(nullptr);
    end->set_next_bundle(nullptr);

    for(SharedBundle kk = begin; (kk != nullptr);
        kk = kk->get_next_bundle() )
    {
        kk->SetRelativePosition(v);
    }
//    if(begin != nullptr)
//        begin->SetRelativePosition(v);
//    if(end != nullptr)
//        end->SetRelativePosition(v);
    SharedBundle j;
    SharedBundle i;
    int num_intersections = 0;

    for(i = begin;
        /*(i != end->get_next_bundle()) && */(i != nullptr);
        i = i->get_next_bundle())
    {
        j = i;

        while(
              (j->get_prev_bundle() != nullptr) &&
              (j->get_rel_position() < j->get_prev_bundle()->get_rel_position())
              )
        {
            num_intersections += (j->CountSegments())*
                    (j->get_prev_bundle()->CountSegments());
            SwapAdjacentBundles(j->get_prev_bundle(), j);
           // j = j->get_prev_bundle();
        }
    }

    if(j != nullptr)
    {
        begin = j;
        while(begin->get_prev_bundle() != nullptr) begin = begin->get_prev_bundle();
    }
    if(i != nullptr)
    {
        end = i;
        while(end->get_next_bundle() != nullptr) end = end->get_next_bundle();
    }

    begin->set_prev_bundle(temp_prev);
    end->set_next_bundle(temp_next);

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

//    if(a->get_prev_bundle() != nullptr)
//        a->get_prev_bundle()->set_next_bundle(b);
//    if(a->get_next_bundle() != nullptr)
//        a->get_next_bundle()->set_prev_bundle(b);
//    if(b->get_prev_bundle() != nullptr)
//        b->get_prev_bundle()->set_next_bundle(a);
//    if(b->get_next_bundle() != nullptr)
//        b->get_next_bundle()->set_prev_bundle(a);
//    tmp = a->get_prev_bundle();
//    a->set_prev_bundle(b->get_prev_bundle());
//    b->set_prev_bundle(tmp);
//    tmp = a->get_next_bundle();
//    a->set_next_bundle(b->get_next_bundle());
//    b->set_next_bundle(tmp);
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
