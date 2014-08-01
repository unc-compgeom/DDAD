#include "arithmetic.h"
#include "line.h"
#include "intersection.h"
#include "point.h"
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
    // Generate horizontal sentinels
    //  (order is red-blue-red-...-blue-red-blue from bottom to top)
    bdl.GenerateSentinels(L, bdt);
    Bundle* red_above = bdl.top_->next_bundle_;
    Bundle* red_below = bdl.bottom_;
    Bundle* blue_above = bdl.top_;
    Bundle* blue_below = bdl.bottom_->prev_bundle_;
    Bundle* bot = bdl.bottom_;
    Bundle* top = bdl.top_;

    //for each event p in L do:
    int endpoint_index = 0; //debug

    for(std::list<ArrangementVertex_2r>::iterator ii = L.begin();
        ii != L.end(); ii ++)
    {

        std::cout << "\nCurrent vertex: " << *(ii->get_point());

        // Locates the highest bundle below or containing the current event
        bdl.LocateVertex(*ii, bdt, red_above, red_below,
                                 blue_above, blue_below);
        // Just performing linear search through the list for now
        bot = bdl.bottom_->prev_bundle_;
        top = bdl.top_->next_bundle_;

        // Debug
//        bdl.PrintState(bdl.bottom_, bdl.top_);

        // Split any bundles containing the current vertex (must be in
        //  range [bottom, top]
        bdl.SplitBundlesContaining(*ii, top, bot);

        // Check to make sure bundles are still in the right order
        if(!bdl.CheckCorrectOrder())
            std::cout << "Something wrong here.";


        // Count the crossings witnessed by the event point
        crossings += bdl.SortPortion(*ii, bot, top);

        // Check to make sure bundles are still in the right order
        if(!bdl.CheckCorrectOrder())
            std::cout << "Something wrong here.";

        if(*(ii->get_point()) < *(ii->get_other_point()))
            // Current point is a left-endpoint
            // Search for the point in the linked list
            bdl.InsertLeftEndpoint(*ii, bdt);
        else
            // Point is a right-endpoint, remove its segment from the structure
            // Locate the bundle containing it
            bdl.RemoveRightEndpoint(*ii, bdt);

        // Check to make sure bundles are still in the right order
        if(!bdl.CheckCorrectOrder())
            std::cout << "Something wrong here.";

        // Merge any bundles in the list that deserve it
        bdl.MergeOrderedBundles(bdt);

        if(!bdl.CheckCorrectOrder())
            std::cout << "MergeOrderedBundles: Something wrong here.";
        if(!bdl.CheckInvariants())
            std::cout << "NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO";
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

Bundle::Bundle() : SplayTree<SharedSegment>()
{
    top_segment_ = nullptr;
    bottom_segment_ = nullptr;
    next_bundle_ = nullptr;
    prev_bundle_ = nullptr;
}
Bundle::Bundle(BinaryNode<SharedSegment> *new_root) :
    SplayTree<SharedSegment>(new_root)
{
    top_segment_ = FindMax()->element;
    bottom_segment_ = FindMin()->element;
    next_bundle_ = nullptr;
    prev_bundle_ = nullptr;
}
Bundle::~Bundle()
{
    MakeEmpty();
    top_segment_ = nullptr;
    bottom_segment_ = nullptr;
    next_bundle_ = nullptr;
    prev_bundle_ = nullptr;
}
Bundle::Bundle(SharedSegment& new_segment)
{
    Insert(new_segment);
    next_bundle_ = nullptr;
    prev_bundle_ = nullptr;
}

void Bundle::Splay( SharedSegment x, BinaryNode<SharedSegment> *t)
{

    BinaryNode<SharedSegment> N, *L, *R, *y;
    if(t == nullptr) return;
    N.left = N.right = nullptr;
    L = R = &N;

    if((t->left == nullptr) && (t->right == nullptr)) return;

    while(true){
        if(t->element->IsAbove(*x)){
            if(t->left == nullptr) break;
            if(t->left->element->IsAbove(*x))
            {
                y = t->left;
                t->left = y->right;
                y->right = t;
                t = y;
                if(t->left == nullptr) break;
            }
            R->left = t;
            R = t;
            t = t->left;
        }
        else if(x->IsAbove(*(t->element)))
        {
            if(t->right == nullptr) break;
            if(x->IsAbove(*(t->right->element)))
            {
                y = t->right;
                t->right = y->left;
                y->left = t;
                t = y;
                if(t->right == nullptr) break;
            }
            L->right = t;
            L = t;
            t = t->right;
        }
        else break;
    }

    L->right = t->left;
    R->left = t->right;
    t->left = N.right;
    t->right = N.left;
    root_ = t;

//     Rotate right if we don't yet satisfy the output conditions
    if(root_->left != nullptr){
        if(root_->element->IsAbove(*x) && x->IsAbove(*root_->left->element))
        {
            t = root_->left;
            root_->left = t->right;
            t->right = root_;
            root_ = t;
        }
    }
}

void Bundle::Splay( Point_2r &x, BinaryNode<SharedSegment> *t)
{

    BinaryNode<SharedSegment> N, *L, *R, *y;
    if(t == nullptr) return;
    N.left = N.right = nullptr;
    L = R = &N;

    if((t->left == nullptr) && (t->right == nullptr)) return;

    while(true){
        if(Predicate::AIsRightOfB(x, t->element->support())){
            if(t->left == nullptr) break;
            if(Predicate::AIsRightOfB(x, t->left->element->support()))
            {
                y = t->left;
                t->left = y->right;
                y->right = t;
                t = y;
                if(t->left == nullptr) break;
            }
            R->left = t;
            R = t;
            t = t->left;
        }
        else if(Predicate::AIsLeftOfB(x, t->element->support()))
        {
            if(t->right == nullptr) break;
            if(Predicate::AIsLeftOfB(x, t->right->element->support()))
            {
                y = t->right;
                t->right = y->left;
                y->left = t;
                t = y;
                if(t->right == nullptr) break;
            }
            L->right = t;
            L = t;
            t = t->right;
        }
        else break;
    }

    L->right = t->left;
    R->left = t->right;
    t->left = N.right;
    t->right = N.left;
    root_ = t;

//     Rotate right if we don't yet satisfy the output conditions
    if(root_->left != nullptr){
        if(Predicate::AIsRightOfB(x, root_->element->support()) &&
                Predicate::AIsLeftOfB(x, root_->left->element->support()))
        {
            t = root_->left;
            root_->left = t->right;
            t->right = root_;
            root_ = t;
        }
    }
}

void Bundle::Insert( const SharedSegment & x )
{
    if(x->q() < x->p()){
            Point_2r new_p = x->q();
            Point_2r new_q = x->p();
            bool new_color = x->get_color();
            Insert(std::make_shared<Segment_2r_colored>(new_p, new_q, new_color));
            return;
    }
    BinaryNode<SharedSegment> *newNode = new BinaryNode<SharedSegment>;
    newNode->element = x;

    if( root_ == nullptr )
    {
        newNode->left = newNode->right = nullptr;
        root_ = newNode;
    }
    else
    {
        BinaryNode<SharedSegment> *tmp_root = root_;
        while(tmp_root != nullptr){
            if(tmp_root->element->IsAbove(*x))
            {
                if(tmp_root->left == nullptr){
                    tmp_root->left = newNode;
                    break;
                }
                tmp_root = tmp_root->left;
            }
            else if(x->IsAbove(*(tmp_root->element)))
            {
                if(tmp_root->right == nullptr){
                    tmp_root->right = newNode;
                    break;
                }
                tmp_root = tmp_root->right;
            }
            else return; // No duplicates
        }
        Splay(x, root_);
    }
//     Make sure top_segment and bottom_segment have been set
        if(top_segment_ == nullptr && bottom_segment_ == nullptr){
            top_segment_ = bottom_segment_ = x;
        }
        else{
            if(x->IsAbove(*top_segment_))
                top_segment_ = x;
            if(bottom_segment_->IsAbove(*x))
                bottom_segment_ = x;
        }
}

Bundle* Bundle::Split(Point_2r& split_here)
{
    if(Size() == 1) return this;
    // Splitting at a minimum/maximum value should always leave at least that
    // value in the tree
    Bundle* new_bundle = new Bundle();
    Splay(split_here, root_);
    if(Predicate::AIsAheadOfB(split_here, root_->element->support_ray()) &&
            root_->right == nullptr)
    {
        new_bundle->Insert(root_->element);
        root_ = root_->left;
    }
    else
    {
        new_bundle->root_ = root_->right;
        new_bundle->top_segment_ = new_bundle->FindMax()->element;
        new_bundle->bottom_segment_ = new_bundle->FindMin()->element;
        root_->right = nullptr;
    }
    // Set next_bundle and prev_bundle pointers
    new_bundle->prev_bundle_ = this;
    new_bundle->next_bundle_ = next_bundle_;
    if(next_bundle_ != nullptr) next_bundle_->prev_bundle_ = new_bundle;
    next_bundle_ = new_bundle;
    top_segment_ = FindMax()->element;
    bottom_segment_ = FindMin()->element;
    return new_bundle;
}

void Bundle::Merge(Bundle* to_merge)
{
    if(top_segment_->IsAbove(*(to_merge->bottom_segment_)))
        std::cout << "Merge: NOOOOOOOOOOOOOOOOOOO";
    if(to_merge->next_bundle_!= nullptr){
        to_merge->next_bundle_->prev_bundle_= this;
    }
    next_bundle_ = to_merge->next_bundle_;
    top_segment_ = to_merge->top_segment_;
    if(is_empty()){
        root_ = to_merge->root_;
        return;
    }
    FindMax();  // Splay max(L) to top
    root_->right = to_merge->root_;
}

bool Bundle::ContainsValue(const SharedSegment &x)
{
    if (is_empty()) return false;
    BinaryNode<SharedSegment>* tmp_root = root_;
    while(tmp_root != nullptr){
        if(tmp_root->element->IsAbove(*x))
            tmp_root = tmp_root->left;
        else if(x->IsAbove(*(tmp_root->element)))
            tmp_root = tmp_root->right;
        else
            return true;
    }
    return false;
}

bool Bundle::Contains(const ArrangementVertex_2r &test_point)
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

bool Bundle::Remove(const SharedSegment &x)
{
    if(is_empty()) return false; // Can't remove from an empty tree
    BinaryNode<SharedSegment> *new_node;

    if( !ContainsValue(x) )
        return false;   // Item not found; do nothing
        // If x is found, it will be at the root
    Splay( x, root_ );

    if( root_->left == nullptr )
        new_node = root_->right;
    else
    {
        // Find the maximum in the left subtree
        Bundle* new_tree = new Bundle(root_->left);
        new_tree->Splay(x, new_tree->root_);
        new_node = new_tree->root_;
        new_node->right = root_->right;
    }
//    delete root;
    root_ = new_node;
    return true;
}

//BinaryNode<SharedSegment>* Bundle::FindMax()
//{
//    if( is_empty( ) )
//        return nullptr;

//    BinaryNode<SharedSegment>* ptr = root_;

//    while( ptr->right != nullptr )
//        ptr = ptr->right;

//    Splay( ptr->element, root_ );
//    return root_;
//}

//BinaryNode<SharedSegment>* Bundle::FindMin( )
//{
//    if( is_empty( ) )
//        return nullptr;
//    BinaryNode<SharedSegment> *ptr = root_;
//    while( ptr->left != nullptr )
//        ptr = ptr->left;

//    Splay( ptr->element, root_ );
//    return root_;
//}

//=============================================================================
// Implementation: BundleTree
//=============================================================================

void BundleTree::Splay( Bundle* x, BinaryNode<Bundle*>* t)
{
    BinaryNode<Bundle*> N, *L, *R, *y;
    if(t == nullptr) return;
    N.left = N.right = nullptr;
    L = R = &N;

    if((t->left == nullptr) && (t->right == nullptr)) return;

    while(true){
        if(t->element->bottom_segment_->IsAbove(*(x->top_segment_)))
        {
            if(t->left == nullptr) break;
            if(t->left->element->bottom_segment_->IsAbove(*(x->top_segment_)))
            {
                y = t->left;
                t->left = y->right;
                y->right = t;
                t = y;
                if(t->left == nullptr) break;
            }
            R->left = t;
            R = t;
            t = t->left;
        }
        else if(x->bottom_segment_->IsAbove(*(t->element->top_segment_)))
        {
            if(t->right == nullptr) break;
            if(x->bottom_segment_->IsAbove(*(t->right->element->top_segment_)))
            {
                y = t->right;
                t->right = y->left;
                y->left = t;
                t = y;
                if(t->right == nullptr) break;
            }
            L->right = t;
            L = t;
            t = t->right;
        }
        else break;
    }

    L->right = t->left;
    R->left = t->right;
    t->left = N.right;
    t->right = N.left;
    root_ = t;

//     Rotate right if we don't yet satisfy the output conditions
    if(root_->left != nullptr){
        if(root_->element->bottom_segment_->IsAbove(*(x->top_segment_)) &&
            (x->bottom_segment_->IsAbove(*(root_->left->element->top_segment_))))
        {
            t = root_->left;
            root_->left = t->right;
            t->right = root_;
            root_ = t;
        }
    }
}

void BundleTree::Splay(Point_2r &x, BinaryNode<Bundle*> *t)
{

    BinaryNode<Bundle*> N, *L, *R, *y;
    if(t == nullptr) return;
    N.left = N.right = nullptr;
    L = R = &N;

    if((t->left == nullptr) && (t->right == nullptr)) return;

    while(true){
        if(Predicate::AIsRightOfB(x, t->element->bottom_segment_->support())){
            if(t->left == nullptr) break;
            if(Predicate::AIsRightOfB(x, t->left->element->bottom_segment_->support()))
            {
                y = t->left;
                t->left = y->right;
                y->right = t;
                t = y;
                if(t->left == nullptr) break;
            }
            R->left = t;
            R = t;
            t = t->left;
        }
        else if(Predicate::AIsLeftOfB(x, t->element->top_segment_->support()))
        {
            if(t->right == nullptr) break;
            if(Predicate::AIsLeftOfB(x, t->right->element->top_segment_->support()))
            {
                y = t->right;
                t->right = y->left;
                y->left = t;
                t = y;
                if(t->right == nullptr) break;
            }
            L->right = t;
            L = t;
            t = t->right;
        }
        else break;
    }

    L->right = t->left;
    R->left = t->right;
    t->left = N.right;
    t->right = N.left;
    root_ = t;

//     Rotate right if we don't yet satisfy the output conditions
    if(root_->left != nullptr){
        if(Predicate::AIsRightOfB(x, root_->element->bottom_segment_->support()) &&
                Predicate::AIsLeftOfB(x, root_->left->element->top_segment_->support()))
        {
            t = root_->left;
            root_->left = t->right;
            t->right = root_;
            root_ = t;
        }
    }
}

void BundleTree::Insert(Bundle* new_bundle)
{
    BinaryNode<Bundle*> *newNode = new BinaryNode<Bundle*>;
    newNode->element = new_bundle;

    if(!new_bundle->get_color()) return;    //don't insert if the bundle is blue

    if( root_ == nullptr )
    {
        newNode->left = newNode->right = nullptr;
        root_ = newNode;
    }
    else
    {
        BinaryNode<Bundle*> *tmp_root = root_;
        while(tmp_root != nullptr){
            if(tmp_root->element->bottom_segment_->IsAbove(*(new_bundle->top_segment_)))
            {
                if(tmp_root->left == nullptr){
                    tmp_root->left = newNode;
                    break;
                                           }
                tmp_root = tmp_root->left;
            }
            else if(new_bundle->bottom_segment_->IsAbove(*(tmp_root->element->top_segment_)))
            {
                if(tmp_root->right == nullptr){
                    tmp_root->right = newNode;
                    break;
                }
                tmp_root = tmp_root->right;
            }
            else return; // No duplicates
        }
        Splay(new_bundle, root_);
    }
}

bool BundleTree::Remove(Bundle* remove_this)
{
    if(root_ == nullptr) return false; // Can't remove from an empty tree
    BinaryNode<Bundle*> *new_node;

    if( !ContainsValue(remove_this) )
        return false;   // Item not found; do nothing
        // If x is found, it will be at the root
    Splay( remove_this, root_ );
//    assert(root_->left == nullptr);
    if( root_->left == nullptr )
        new_node = root_->right;
    else
    {
        // Find the maximum in the left subtree
        BundleTree* new_tree = new BundleTree();
        new_tree->root_ = root_->left;
//        SplayTree<Bundle*>* new_tree = new SplayTree<Bundle*>(root_->left);
        new_tree->Splay(remove_this, new_tree->root_);
        new_node = new_tree->root_;
        new_node->right = root_->right;
        delete new_tree;
    }
//    delete root;
    root_ = new_node;
    return true;
}

void BundleTree::SplitAtVertex(const ArrangementVertex_2r input_vertex)
{
    Splay(*input_vertex.get_point(),root_);
    Bundle* new_bundle = root_->element->Split(*input_vertex.get_point());
    Insert(new_bundle);
}

bool BundleTree::ContainsValue(Bundle * const &x)
{
    if (is_empty()) return false;
    BinaryNode<Bundle*>* tmp_root = root_;
    while(tmp_root != nullptr){
        if(Predicate::AIsRightOfB(x->top_segment_->p(), tmp_root->element->bottom_segment_->support()))
            tmp_root = tmp_root->left;
        else if(Predicate::AIsLeftOfB(x->bottom_segment_->p(), tmp_root->element->top_segment_->support()))
            tmp_root = tmp_root->right;
        else if(x == tmp_root->element)
            return true;
        else
            return true;
    }
    return false;
}


//BinaryNode<Bundle*>* BundleTree::FindMax()
//{
//    if( is_empty( ) )
//        return nullptr;

//    BinaryNode<Bundle*>* ptr = root_;

//    while( ptr->right != nullptr )
//        ptr = ptr->right;

//    Splay( (ptr->element), root_ );
//    return root_;
//}

//BinaryNode<Bundle*>* BundleTree::FindMin( )
//{
//    if( is_empty( ) )
//        return nullptr;
//    BinaryNode<Bundle*> *ptr = root_;
//    while( ptr->left != nullptr )
//        ptr = ptr->left;

//    Splay( (ptr->element), root_ );
//    return root_;
//}


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
        (order is red-blue-red-...-blue-red-blue from bottom to top) */
    Bundle* top_blue_cap = new Bundle();
    Segment_2r_colored tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, max_y+3),
                std::make_shared<Point_2r>(max_x, max_y+3),
                false);
    top_blue_cap->Insert(std::make_shared<Segment_2r_colored>(tmp_segment));

    Bundle* top_red_cap = new Bundle();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, max_y+4),
                std::make_shared<Point_2r>(max_x, max_y+4),
                true);
    top_red_cap->Insert(std::make_shared<Segment_2r_colored>(tmp_segment));

    Bundle* top_red_sentinel = new Bundle();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, max_y+(2)),
                std::make_shared<Point_2r>(max_x, max_y+(2)),
                true);
    top_red_sentinel->Insert
            (std::make_shared<Segment_2r_colored>(tmp_segment));
    //    std::cout << "\nTop red: " << tmp_segment.p() << tmp_segment.q();

    Bundle* top_blue_sentinel = new Bundle();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, max_y+(1)),
                std::make_shared<Point_2r>(max_x, max_y+(1)),
                false);
    top_blue_sentinel->Insert
            (std::make_shared<Segment_2r_colored>(tmp_segment));
    //    std::cout << "\nTop blue: " << tmp_segment.p() << tmp_segment.q();

    Bundle* bot_red_sentinel = new Bundle();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, min_y+(-1)),
                std::make_shared<Point_2r>(max_x, min_y+(-1)),
                true);
    bot_red_sentinel->Insert
            (std::make_shared<Segment_2r_colored>(tmp_segment));
    //    std::cout << "\nBot red: " << tmp_segment.p() << tmp_segment.q();

    Bundle* bot_blue_sentinel = new Bundle();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, min_y+(-2)),
                std::make_shared<Point_2r>(max_x, min_y+(-2)),
                false);
    bot_blue_sentinel->Insert
            (std::make_shared<Segment_2r_colored>(tmp_segment));
    //    std::cout << "\nBot blue: " << tmp_segment.p() << tmp_segment.q();

    Bundle* bot_red_cap = new Bundle();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, min_y-3),
                std::make_shared<Point_2r>(max_x, min_y-3),
                true);
    bot_red_cap->Insert(std::make_shared<Segment_2r_colored>(tmp_segment));

    Bundle* bot_blue_cap = new Bundle();
    tmp_segment = Segment_2r_colored(
                std::make_shared<Point_2r>(min_x, min_y-4),
                std::make_shared<Point_2r>(max_x, min_y-4),
                false);
    bot_blue_cap->Insert(std::make_shared<Segment_2r_colored>(tmp_segment));

    Insert(bot_blue_cap, nullptr);
    Insert(bot_red_cap, bot_blue_cap);
    Insert(bot_blue_sentinel, bot_red_cap);
    Insert(bot_red_sentinel, bot_blue_sentinel);
    Insert(top_blue_sentinel, bot_red_sentinel);
    Insert(top_red_sentinel, top_blue_sentinel);
    Insert(top_blue_cap, top_red_sentinel);
    Insert(top_red_cap, top_blue_cap);
    bdt.Insert(bot_red_cap);
    bdt.Insert(bot_red_sentinel);
    bdt.Insert(top_red_sentinel);
    bdt.Insert(top_red_cap);
    bottom_ = (bot_red_sentinel);
    top_ = (top_blue_sentinel);
}

void BundleList::Insert(Bundle* insert_this, Bundle* after_this)
{
    if(after_this == nullptr)
    {
        insert_this->next_bundle_ = bottom_;
        bottom_ = insert_this;
    }
    else
    {
        insert_this->next_bundle_ = after_this->next_bundle_;
        after_this->next_bundle_ = insert_this;
    }

    insert_this->prev_bundle_ = (after_this);
    Bundle* temp = insert_this->next_bundle_;

    if(temp == nullptr) top_ = insert_this;
    else temp->prev_bundle_ = (insert_this);

}

void BundleList::Remove(Bundle* remove_this)
{
    assert(remove_this != nullptr);

    if(top_ == remove_this)
        top_ = remove_this->prev_bundle_;
    if(bottom_ == remove_this)
        bottom_ = remove_this->next_bundle_;

    Bundle* prev_bundle = remove_this->prev_bundle_;
    Bundle* next_bundle = remove_this->next_bundle_;

    if(prev_bundle != nullptr)
        prev_bundle->next_bundle_ = (next_bundle);
    if(next_bundle != nullptr)
        next_bundle->prev_bundle_ = (prev_bundle);
//    delete remove_this;
}

void BundleList::LocateVertex(ArrangementVertex_2r &input_vertex,
                              BundleTree& bdt,
                              Bundle*& red_above,
                              Bundle*& red_below,
                              Bundle*& blue_above,
                              Bundle*& blue_below)
{
    // Using the tree and list, locate the highest bundle below or containing
    //  the input vertex
    // Locate red bundles within the tree
    bdt.Splay(*input_vertex.get_point(), bdt.root_);
    red_below = bdt.root_->element;
    red_above = red_below->next_bundle_->next_bundle_;
    if(red_below->Contains(input_vertex))
        red_below = red_below->prev_bundle_->prev_bundle_;

    // Locate blue bundles using the list
    blue_below = red_below->next_bundle_;
    blue_above = red_below->prev_bundle_;
    while(blue_below->SetRelativePosition(input_vertex) != BELOW)
        blue_below = blue_below->prev_bundle_->prev_bundle_;
    while(blue_above->SetRelativePosition(input_vertex) != ABOVE)
       blue_above = blue_above->next_bundle_->next_bundle_;
}

Bundle* BundleList::SplitBundleAtVertex(Bundle* split_bundle,
                                             ArrangementVertex_2r &here)
{
    if(split_bundle->Size()==1) return split_bundle;
    Bundle* new_bundle = split_bundle->Split(*(here.get_point()));
    if(split_bundle == top_) top_ = new_bundle;
    return new_bundle;
}

void BundleList::SplitBundlesContaining(ArrangementVertex_2r& input_vertex,
                                        Bundle* top,
                                        Bundle* bot)
{
    for(Bundle* ii = bot; ii != top; ii = ii->next_bundle_)
    {
        if(ii->Contains(input_vertex))
        {
            if(ii->get_color())
                ii->Split(*input_vertex.get_point());
            else
                SplitBundleAtVertex(ii, input_vertex);
            if(top_ == ii)
                top_ = ii->next_bundle_;
        }
    }
}

int BundleList::SortPortion(ArrangementVertex_2r& v, Bundle* begin,
                            Bundle* end)
{
    for(Bundle* kk = begin; (kk != end->next_bundle_);
        kk = kk->next_bundle_ )
    {
        kk->SetRelativePosition(v);
    }
    Bundle* j;
    Bundle* i;
    int num_intersections = 0;
    Bundle* begin_b = begin->prev_bundle_;
    Bundle* end_b = end->next_bundle_;
    i = begin;
    while(i != end_b)
    {
        j = i;
        i = i->next_bundle_;
        while(
              (j->prev_bundle_ != begin_b) &&
              (j->rel_position_ < j->prev_bundle_->rel_position_)
              )
        {
            num_intersections += (j->Size())*(j->prev_bundle_->Size());
            SwapAdjacentBundles(j->prev_bundle_, j);
        }
    }

    return num_intersections;
}

void BundleList::SwapAdjacentBundles(Bundle* left, Bundle* right)
{
    assert(left != right);
    assert(left->next_bundle_ == right);
    assert(right->prev_bundle_ == left);
    if(left == bottom_)
        bottom_ = right;
    else if(right == bottom_)
        bottom_ = left;
    if(left == top_)
        top_ = right;
    else if(right == top_)
        top_ = left;
    Bundle* tmp = left->prev_bundle_;
    left->prev_bundle_ = (right);
    left->next_bundle_ = (right->next_bundle_);
    if(left->next_bundle_ != nullptr)
        left->next_bundle_->prev_bundle_ = (left);
    right->prev_bundle_ = (tmp);
    right->next_bundle_ = (left);
    if(right->prev_bundle_ != nullptr)
        right->prev_bundle_->next_bundle_ = (right);
}

void BundleList::InsertLeftEndpoint(ArrangementVertex_2r& input_vertex,
                                    BundleTree& bdt)
{

    Bundle* new_bundle = new Bundle();
    Segment_2r_colored current_segment =
            Segment_2r_colored(input_vertex.get_point(),
                               input_vertex.get_other_point(),
                               input_vertex.is_red());
    SharedSegment new_segment =
            std::make_shared<Segment_2r_colored>(current_segment);
    new_bundle->Insert(new_segment);
    bdt.Splay(new_bundle, bdt.root_);
    Bundle* tmp = bdt.root_->element->next_bundle_;
    while(tmp->bottom_segment_->IsAbove(*new_segment))
        tmp = tmp->prev_bundle_;
    if(input_vertex.is_red())
        bdt.Insert(new_bundle);
    Insert(new_bundle, tmp);

    if(!CheckCorrectOrder())
        std::cout << "InsertLeftEndpoint: Something wrong here.";
}

void BundleList::RemoveRightEndpoint(ArrangementVertex_2r& input_vertex,
                                     BundleTree& bdt)
{
    for(Bundle* jj = bottom_;
        (jj != nullptr);
        jj = jj->next_bundle_)
    {
        if(jj->Contains(input_vertex))
        {
            SharedSegment current_segment = input_vertex.get_segment();
            jj->Remove(current_segment);
            if(jj->Size() == 0)
            {
                bdt.Remove(jj);
                Remove(jj);
            }
        }
    }
    if(!CheckCorrectOrder())
        std::cout << "RemoveRightEndpoint: Something wrong here.";
}

void BundleList::MergeOrderedBundles(BundleTree& bdt)
{
    for(Bundle* jj = bottom_; jj != top_->next_bundle_; )
    {

        Bundle* tmp = new Bundle();
        while(jj->get_color() == jj->next_bundle_->get_color())
        {
            tmp = jj->next_bundle_;
            //deal with bundle tree if bundles are red
            if(jj->get_color())
            {
                bdt.Remove(jj);
                bdt.Remove(tmp);
                jj->Merge(tmp);
                bdt.Insert(jj);
            }
            else
                jj->Merge(jj->next_bundle_);
            // reset top if need be
            if(tmp == top_)
                top_ = jj;
            if(!CheckCorrectOrder())
                std::cout << "MergeOrderedBundles: Something wrong here.";
        }
        jj = jj->next_bundle_;
    }
}

void BundleList::PrintState(Bundle* start, Bundle* end)
{
    std::string current_color, bp, bq, tp, tq;
    int nsegments;
    for(Bundle* tmp = start; tmp != end->next_bundle_; tmp = tmp->next_bundle_)
    {
        current_color = (tmp->get_color()) ? "RED" : "BLUE";
        tp = to_string(tmp->top_segment_->p());
        tq = to_string(tmp->top_segment_->q());
        bp = to_string(tmp->bottom_segment_->p());
        bq = to_string(tmp->bottom_segment_->q());
        nsegments =tmp->Size();
        printf("\n%8.8s -> %8.8s | %8.8s -> %8.8s | %4.4s | %2.2i",
               bp.c_str(), bq.c_str(), tp.c_str(), tq.c_str(),
               current_color.c_str(), nsegments);
    }
    printf("\n");
}

bool BundleList::CheckInvariants()
{
    Bundle* next_same_color;
    for(Bundle* ii = bottom_; ii != top_->next_bundle_; ii = ii->next_bundle_)
    {
        if(ii->next_bundle_->get_color() == ii->get_color()) return false;
        if(ii->prev_bundle_->get_color() == ii->get_color()) return false;
        if(ii->next_bundle_->next_bundle_ != nullptr)
        {
            next_same_color = ii->next_bundle_->next_bundle_;
            if(ii->top_segment_->IsAbove(*(next_same_color->bottom_segment_)))
                return false;
        }
    }
    return true;
}

bool BundleList::CheckCorrectOrder() const
{
    BundleList blue_list = BundleList();
    BundleList red_list = BundleList();
    // Separate out red and blue lists
    for(Bundle* ii = top_; ii != bottom_; ii = ii->prev_bundle_)
    {
        Bundle* bundle_copy = new Bundle(*ii);
        if(ii->get_color()) red_list.Insert(bundle_copy, nullptr);
        else blue_list.Insert(bundle_copy, nullptr);
    }

    for(Bundle* jj = blue_list.bottom_; jj != blue_list.top_; jj = jj->next_bundle_)
    {
        if(jj->top_segment_->IsAbove(*(jj->next_bundle_->top_segment_)))
            return false;
    }
    for(Bundle* kk = red_list.bottom_; kk != red_list.top_; kk = kk->next_bundle_)
    {
        if(kk->top_segment_->IsAbove(*(kk->next_bundle_->bottom_segment_)))
            return false;
    }
    return true;
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
