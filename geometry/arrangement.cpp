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
    std::list<Arrangement_Vertex_2r> L = A.vertices();
    //vertex comparator
    struct compare{
                    bool operator()(Arrangement_Vertex_2r a, Arrangement_Vertex_2r b){
                        if(a.getX() < b.getX()){return true;}
                        if(a.getX() > b.getX()){return false;}
                        return a.getY() < b.getY();
                    }
                };
    //sort vertices lexicographically
    L.sort(compare());

    // Initialize data structures
    Bundle_Tree bdt = Bundle_Tree();
    Bundle_List bdl = Bundle_List();
    Arrangement_Bundle top_red;
    Arrangement_Bundle bot_red;
    Arrangement_Bundle top_blue;
    Arrangement_Bundle bot_blue;
    RelativePosition top_red_position;
    RelativePosition bot_red_position;
    RelativePosition top_blue_position;
    RelativePosition bot_blue_position;

    //for each event p in L do:

    for(std::list<Arrangement_Vertex_2r>::iterator ii = L.begin(); ii != L.end(); ii ++){
        bdt.LocatePoint(*ii, top_red, bot_red);
        bdl.LocatePoint(*ii, top_blue, bot_blue);
        top_red_position = top_red.relPosition(*ii);
        bot_red_position = bot_red.relPosition(*ii);
        top_blue_position = top_blue.relPosition(*ii);
        bot_blue_position = bot_blue.relPosition(*ii);
        //	split(p, brundletree, red)
        //	split(p, brundlelist, blue)
        Segment_2r_colored current_segment = Segment_2r_colored(ii->getPoint(), ii->getOtherPoint(), ii->getColor());

        //	if brundles r and b in wrong positions:
        //		countIntersections(rxb)
        //		swap(r,b)
        // repair brundletree by merge
        // repair brundlelist
        // Insert any new bundles
        if(ii->getPoint() < ii->getOtherPoint()){ // Current point is a left-endpoint
            // Search for the point in the linked list
            Arrangement_Bundle new_bundle = Arrangement_Bundle();
            new_bundle.insert(current_segment);
            std::list<Arrangement_Bundle>::iterator pos_in_list = bdl.FindIndex(current_segment);
            bdl.Insert(pos_in_list, new_bundle);
        }
        else{ // Current point is a right-endpoint, pop the segment
            // First from the bundle tree
            Arrangement_Bundle tmp_bundle = Arrangement_Bundle();
            tmp_bundle.insert(current_segment);
            bdt.find(tmp_bundle);
            bdt.getRoot()->element.remove(current_segment);
            if(bdt.getRoot()->element.isEmpty()) bdt.remove(bdt.getRoot()->element);
            // Then from the list
            std::list<Arrangement_Bundle>::iterator bundle_it = bdl.FindIndex(current_segment);
            bundle_it->remove(current_segment);
            if(bundle_it->isEmpty()) bdl.Remove(bundle_it);
        }


        //def split(p, struct, color):
        //	switch(locate p in struct):
        //		case (p between two (color) bundles):
        //			continue
        //		case (p inside (color) bundle):
        //			split bundle at p
        //		case (p inside two (color) bundles):
        //			split bundles at p
        crossings++;
    }


    return crossings;
}


//=============================================================================
// Implementation: Arrangement_Bundle
//=============================================================================

Arrangement_Bundle::Arrangement_Bundle(){
    topSegment_ = nullptr;
    botSegment_ = nullptr;
}

Arrangement_Bundle::Arrangement_Bundle(SplayTree<Segment_2r_colored> &rhs) :
    SplayTree<Segment_2r_colored>(){
//    *this = SplayTree<Segment_2r_colored>(rhs);
    rhs.findMax();
    topSegment_ = &(rhs.getRoot()->getElement());
    rhs.findMin();
    botSegment_ = &(rhs.getRoot()->getElement());

}

void Arrangement_Bundle::insert(const Segment_2r_colored &x){
    if(x.q() < x.p()){
        Point_2r newp = x.q();
        Point_2r newq = x.p();
        bool newcolor = x.isRed();
        SplayTree<Segment_2r_colored>::insert(Segment_2r_colored(newp, newq, newcolor));
    }
    else SplayTree<Segment_2r_colored>::insert(x);
}

bool Arrangement_Bundle::contains(const Arrangement_Vertex_2r &vert){
    findMax();
    Segment_2r_colored topseg = getRoot()->getElement();
    findMin();
    Segment_2r_colored botseg = getRoot()->getElement();
    return (Predicate::AIsLeftOfB(vert.position(), botseg.support()) && Predicate::AIsRightOfB(vert.position(), topseg.support()));
}

RelativePosition Arrangement_Bundle::relPosition(const Arrangement_Vertex_2r& p){
    // Return ABOVE if the bundle is above p,
    // return BELOW if the bundle is below p,
    // return ENDING if the bundle is ending at p
    if(topSegment_->q() == p.position() || botSegment_->q() == p.position() ||
            topSegment_->p() == p.position() || botSegment_->p() == p.position()){
        return ENDING;
    }
    if(Predicate::AIsRightOfB(p.position(), botSegment_->support())) return ABOVE;
    if(Predicate::AIsLeftOfB(p.position(),topSegment_->support())) return BELOW;
    else return INDETERMINATE;
}


//=============================================================================
// Implementation: Bundle_Tree
//=============================================================================


void Bundle_Tree::LocatePoint(Arrangement_Vertex_2r &p, Arrangement_Bundle &top, Arrangement_Bundle &bottom){
    // Splits bundles in the bundle tree at current vertex, then locates the current vertex among the red bundles in the bundle tree.
    // top = lowest bundle above or ending at p
    // bot = highest bundle below or ending at p

    splitBundles(p);
    Arrangement_Bundle tmpBundle = Arrangement_Bundle();
    tmpBundle.insert(Segment_2r_colored(p.getPoint(), p.getPoint(), p.getColor()));
    find(tmpBundle);  // Splay to p

    // FindMax(left subtree of root)
        // go to root.left, traverse right until you hit a nullptr
        // bot = current
    // FindMin(right subtree of root)
        // go to root.right, traverse left until you hit a nullptr
        // top = current
    // Remove P from the tree

}

void Bundle_Tree::splitBundles(Arrangement_Vertex_2r &p){
    if(root == nullptr) return;
    Arrangement_Bundle tmpBundle = Arrangement_Bundle();
    tmpBundle.insert(Segment_2r_colored(p.getPoint(), p.getPoint(), p.getColor()));
    find(tmpBundle);
    Segment_2r_colored topSeg = *(root->getElement().getTop());    //top segment of bundle below or containing p
    Segment_2r_colored botSeg;   //bottom segment of bundle above or containing p

    if(Predicate::AIsRightOfB(p.position(),topSeg.support())){
        //p is in the bottom bundle
        Arrangement_Bundle R = Arrangement_Bundle(root->getElement().splitTree(Segment_2r_colored(p.getPoint(), p.getOtherPoint(), p.getColor())));
        BinaryNode<Arrangement_Bundle>* new_node = new BinaryNode<Arrangement_Bundle>(R, nullptr, root->right);
        root->right = new_node;
        return;
    }

    BinaryNode<Arrangement_Bundle>* t = root->right;
    while (t->left != nullptr){
        t = t->left;
    }
    botSeg = *(t->getElement().getBot());

    if(Predicate::AIsLeftOfB(p.position(),botSeg.support())){
        //p is in the top bundle
        Arrangement_Bundle R = t->getElement().splitTree(Segment_2r_colored(p.getPoint(), p.getOtherPoint(), p.getColor()));
        BinaryNode<Arrangement_Bundle>* new_node = new BinaryNode<Arrangement_Bundle>(R, nullptr, root->right);
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
// Implementation: Bundle_List
//=============================================================================
void Bundle_List::Remove(const std::list<Arrangement_Bundle>::iterator where){
    // Removes the element at location "where"
    blist_.erase(where);
}

void Bundle_List::Insert(std::list<Arrangement_Bundle>::iterator where, Arrangement_Bundle& to_insert){
    blist_.insert(where, to_insert);
}

void Bundle_List::LocatePoint(Arrangement_Vertex_2r &p, Arrangement_Bundle &top, Arrangement_Bundle &bottom){

}

void Bundle_List::SplitBundles(Arrangement_Vertex_2r &p){
    // Do a linear search through the bundles in the list for a bundle containing p
    for(std::list<Arrangement_Bundle>::iterator ii = blist_.begin(); ii != blist_.end(); ii ++){
        if((*ii).contains(p)){
            Arrangement_Bundle R = (*ii).splitTree(Segment_2r_colored(p.getPoint(), p.getOtherPoint(), p.getColor()));
            blist_.insert(++ii, R);
            return;
        }
    }
}
std::list<Arrangement_Bundle>::iterator Bundle_List::FindIndex(const Segment_2r_colored& target_segment){
    // Return the index of the largest bundle that is below or containing target_segment
    for(std::list<Arrangement_Bundle>::iterator ii = blist_.begin(); ii != blist_.end(); ii ++){
        //check if target_segment is in current bundle
        if(Predicate::AIsRightOfB(target_segment.p(), ii->getTop()->support()) && Predicate::AIsLeftOfB(target_segment.p(), ii->getBot()->support())){
            return ii;
        }
        else if(Predicate::AIsRightOfB(target_segment.p(), std::next(ii)->getBot()->support())){
            return ii;
        }
    }
    return blist_.begin();

}


//=============================================================================
// Implementation: Arrangement_Vertex_2r
//=============================================================================

Arrangement_Vertex_2r::Arrangement_Vertex_2r(SharedPoint_2r &pt, SharedPoint_2r otherPt, bool color){
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
    vertices_.push_front(Arrangement_Vertex_2r(std::make_shared<Point_2r>(v), std::make_shared<Point_2r>(w), color));
    vertices_.push_front(Arrangement_Vertex_2r(std::make_shared<Point_2r>(w), std::make_shared<Point_2r>(v), color));
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

const std::list<Arrangement_Vertex_2r>& Arrangement_2r::vertices() const{
    return vertices_;
}

} // Namespace DDAD
