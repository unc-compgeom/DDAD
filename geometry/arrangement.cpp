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

//L = sort list of endpoints
//for each event p in L do:
//	split(p, brundletree, red)
//	split(p, brundlelist, blue)
//	if brundles r and b in wrong positions:
//		countIntersections(rxb)
//		swap(r,b)
//		repair brundletree by merge
//		repair brundlelist


//def split(p, struct, color):
//	switch(locate p in struct):
//		case (p between two (color) bundles):
//			continue
//		case (p inside (color) bundle):
//			split bundle at p
//		case (p inside two (color) bundles):
//			split bundles at p

    return 0;
}


//=============================================================================
// Implementation: Arrangement_2r
//=============================================================================

Arrangement_2r::~Arrangement_2r() {
    for(auto i = begin(segments_); i != end(segments_); ++i) {
        SigPopVisualPoint_2r(i->p());
        SigPopVisualPoint_2r(i->q());
        SigPopVisualSegment_2r(*i);
    }
}

void Arrangement_2r::AddSegment(Point_2r& v, Point_2r& w, bool color){
    LOG(INFO) << "in AddSegment";
    segments_.push_front(Segment_2r_colored(v, w, color));
    LOG(INFO) << "Pushed a segment to the list";
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
    LOG(INFO) << "Pushed a segment";
}

void Arrangement_2r::EndSegment(Point_2r &v){
    AddSegment(floater_, v, current_color_);
}

void Arrangement_2r::PushPoint(Point_2r &v, bool color){
    PushPoint(std::make_shared<Point_2r>(v), color);
}

void Arrangement_2r::PushPoint(SharedPoint_2r v, bool color){
    LOG(INFO) << "Pushed point";
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

} // Namespace DDAD
